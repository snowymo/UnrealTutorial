/* Copyright (c) 2014-2018 by Mercer Road Corp
 *
 * Permission to use, copy, modify or distribute this software in binary or source form
 * for any purpose is allowed only under explicit prior consent in writing from Mercer Road Corp
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND MERCER ROAD CORP DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL MERCER ROAD CORP
 * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include "ShooterGame.h"
#include "Online/ShooterPlayerState.h"
#include "Misc/NetworkVersion.h"
#include "Vivox/VivoxHUD.h"
#include "Vivox/VivoxGameInstance.h"

typedef TPair<const UFont*, FText> InstructionText;

AVivoxHUD::AVivoxHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> HUDVivoxTextureOb(TEXT("/Game/UI/HUD/Vivox/HUDVivox"));

    // Fonts are not included in dedicated server builds.
#if !UE_SERVER
    {
        static ConstructorHelpers::FObjectFinder<UFont> VivoxH1FontOb(TEXT("/Game/UI/HUD/Vivox/RobotoBlack32"));
        static ConstructorHelpers::FObjectFinder<UFont> VivoxH2FontOb(TEXT("/Game/UI/HUD/Vivox/RobotoMedium24"));
        static ConstructorHelpers::FObjectFinder<UFont> VivoxNormalFontOb(TEXT("/Game/UI/HUD/Vivox/RobotoRegular18"));
        VivoxH1Font = VivoxH1FontOb.Object;
        VivoxH2Font = VivoxH2FontOb.Object;
        VivoxNormalFont = VivoxNormalFontOb.Object;
    }
#endif //!UE_SERVER

    HUDVivoxTexture = HUDVivoxTextureOb.Object;

    VivoxRosterBg = UCanvas::MakeIcon(HUDVivoxTexture, 1, 1, 253, 60);
    VivoxRosterRedTeam  = UCanvas::MakeIcon(HUDVivoxTexture, 1, 65, 73, 56);
    VivoxRosterBlueTeam = UCanvas::MakeIcon(HUDVivoxTexture, 1, 130, 73, 56);
    VivoxRosterPositional = UCanvas::MakeIcon(HUDVivoxTexture, 74, 65, 73, 56);
    VivoxRosterChannelIcon = UCanvas::MakeIcon(HUDVivoxTexture, 1, 195, 49, 46);
    VivoxRosterMutedIcon = UCanvas::MakeIcon(HUDVivoxTexture, 50, 195, 55, 46);

    VivoxPTTPanelBg = UCanvas::MakeIcon(HUDVivoxTexture, 147, 70, 76, 76);
    VivoxPTTPanelActive = UCanvas::MakeIcon(HUDVivoxTexture, 106, 211, 22, 36);
    VivoxPTTPanelOverlay = UCanvas::MakeIcon(HUDVivoxTexture, 130, 211, 22, 36);

    CachedDebugInfoBoxSize = FVector2D(-1, -1);
    CachedTeamColor = FLinearColor::Transparent;

    // Set in constructor we don't reassign every time we draw the HUD. Check flags however, to protect from an Editor crash
    // when it loads default objects and prototypes for the reflection system (in these objects the World is not available).
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
    {
        VivoxGameInstance = GetWorld()->GetGameInstanceChecked<UVivoxGameInstance>();
    }
}

/**
 * Brief Unbinds delegate for telling other methods when players have started/stopped talking to prevent speaking indicators from showing on the scoreboard.
 */
void AVivoxHUD::PostInitializeComponents()
{
    // skip the base class AShooterHUD::PostInitializeComponents method to avoid delegate binding that enables speaking indicators to show on the scoreboard.
    AHUD::PostInitializeComponents();

    bIsScoreBoardVisible = false;
}

FString AVivoxHUD::GetGameplayMode()
{
    if (CachedGameplayMode.IsEmpty()) // cache gamemode so we don't need to do this every frame
    {
        IOnlineSubsystem * OnlineSubsystem = IOnlineSubsystem::Get();
        if (OnlineSubsystem)
        {
            IOnlineSessionPtr SessionSubsystem = OnlineSubsystem->GetSessionInterface();
            if (SessionSubsystem.IsValid())
            {
                FNamedOnlineSession* Session = SessionSubsystem->GetNamedSession(NAME_GameSession);
                if (Session)
                {
                    Session->SessionSettings.Get(SETTING_GAMEMODE, CachedGameplayMode);
                }
            }
        }
    }
    return CachedGameplayMode;
}

FVector2D AVivoxHUD::GetDebugInfoBoxSize()
{
    if (-1 == CachedDebugInfoBoxSize.X) // cache Debug Info box width so we don't need to do this every frame
    {
        if (GetNetMode() != NM_Standalone)
        {
            FString NetModeDesc = (GetNetMode() == NM_Client) ? TEXT("Client") : TEXT("Server");
            IOnlineSubsystem * OnlineSubsystem = IOnlineSubsystem::Get();
            if (OnlineSubsystem)
            {
                IOnlineSessionPtr SessionSubsystem = OnlineSubsystem->GetSessionInterface();
                if (SessionSubsystem.IsValid())
                {
                    FNamedOnlineSession * Session = SessionSubsystem->GetNamedSession(NAME_GameSession);
                    if (Session)
                    {
                        NetModeDesc += TEXT("\nSession: ");
                        NetModeDesc += Session->SessionInfo->GetSessionId().ToString();
                    }
                }
            }
            NetModeDesc += FString::Printf(TEXT("\nVersion: %i, %s, %s"), FNetworkVersion::GetNetworkCompatibleChangelist(), UTF8_TO_TCHAR(__DATE__), UTF8_TO_TCHAR(__TIME__));

            float SizeX, SizeY;
            Canvas->StrLen(NormalFont, NetModeDesc, SizeX, SizeY);

            const float BoxPadding = 5.0f;
            // hack in the *2.f scaling for Y since UCanvas::StrLen doesn't take into account newlines
            const float SCALE_Y = 3.0f;

            CachedDebugInfoBoxSize = FVector2D((SizeX + BoxPadding * SCALE_Y) /** ScaleUI*/, (SizeY * SCALE_Y + BoxPadding * SCALE_Y) /** ScaleUI*/);
        }
        else {
            CachedDebugInfoBoxSize = FVector2D(0, 0);
        }
    }
    return CachedDebugInfoBoxSize;
}

FLinearColor AVivoxHUD::GetTeamColor()
{
    if (CachedTeamColor == FLinearColor::Transparent) // cache team color so we don't need to do this every frame
    {
        AShooterPlayerController* MyPlayerController = Cast<AShooterPlayerController>(PlayerOwner);
        if (MyPlayerController)
        {
            AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(MyPlayerController->PlayerState);
            if (MyPlayerState)
            {
                const int32 TeamRed = 0;
                if (TeamRed == MyPlayerState->GetTeamNum())
                    CachedTeamColor = FLinearColor::FromSRGBColor((FColor(158, 54, 54, 255))); // nice red color
                else
                    CachedTeamColor = FLinearColor::FromSRGBColor((FColor(54, 76, 158, 255))); // nice blue color
            }
        }
    }
    return CachedTeamColor;
}

void AVivoxHUD::DrawVivoxInfoText()
{
#if !UE_BUILD_SHIPPING
    if (GetGameplayMode().Equals(TEXT("TDM"))) // Team Deathmatch
    {
        const float VivoxPosX = Canvas->OrgX + Offset * ScaleUI;
        const float VivoxPosY = Canvas->OrgY + (5 * Offset + GetDebugInfoBoxSize().Y) * ScaleUI;
#if (PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
        FString Text = "Toggle-to-talk: <D-pad Up> for Team Chat and <D-pad Down> for Area Chat";
#else
        FString Text = "Push-to-talk: [F] for Area Chat and [V] for Team Chat";
#endif

        float SizeX, SizeY;
        Canvas->StrLen(NormalFont, Text, SizeX, SizeY);

        const float BoxPadding = 5.0f;
        const float ParagraphPadding = -15.0f;

        FColor DrawColor(HUDDark.R, HUDDark.G, HUDDark.B, HUDDark.A * 0.2f);

        FCanvasTileItem TileItem(FVector2D(VivoxPosX - BoxPadding * ScaleUI, VivoxPosY - BoxPadding * ScaleUI), FVector2D(GetDebugInfoBoxSize().X * ScaleUI, (SizeY + BoxPadding + ParagraphPadding / 2) * ScaleUI), DrawColor);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);

        FCanvasTextItem TextItem(FVector2D(VivoxPosX, VivoxPosY + ParagraphPadding * ScaleUI), FText::FromString(Text), NormalFont, HUDLight);
        TextItem.EnableShadow(FLinearColor::Black);
        TextItem.FontRenderInfo = ShadowedFont;
        TextItem.Scale = FVector2D(ScaleUI, ScaleUI);
        Canvas->DrawItem(TextItem);
    }
#endif

    // instructions if waiting to start match or respawn
    AShooterCharacter* MyPawn = Cast<AShooterCharacter>(GetOwningPawn());
    if (MatchState == EShooterMatchState::Warmup || (MatchState == EShooterMatchState::Playing && !(MyPawn && MyPawn->IsAlive())))
    {
        float TextScale = 1.00f;
        float TextOffsetX = Canvas->ClipX - Canvas->OrgX - Offset;
        float TextOffsetY = Canvas->ClipY - Canvas->OrgY - Offset;

        TArray<InstructionText> Instructions;
        Instructions.Add(InstructionText(VivoxH1Font, FText::FromString(                                              "Vivox Integration")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                                           "")));
#if !(PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
        Instructions.Add(InstructionText(VivoxH2Font, FText::FromString(                                      "Free For All Features ---")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                  "Match Wide 3D Area Chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                                 "Open Mic -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                                           "")));
#endif
        Instructions.Add(InstructionText(VivoxH2Font, FText::FromString(                                   "Team Deathmatch Features ---")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                  "Match Wide 3D Area Chat -")));
#if (PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                  "Team Wide 2D Radio Chat (on by default) -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                         "Toggle-to-Talk (<D-pad Up/down>) -")));
#else
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                  "Team Wide 2D Radio Chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                               "Push-to-Talk ([F] and [V]) -")));
#endif
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                              "Team Roster -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                                           "")));
#if (PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
        Instructions.Add(InstructionText(VivoxH2Font, FText::FromString(                                     "Toggle-To-Talk Buttons ---")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                        "2D Voice On/Off Toggle <D-pad Up> -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                      "3D Voice On/Off Toggle <D-pad Down> -")));
#else
        Instructions.Add(InstructionText(VivoxH2Font, FText::FromString(                                          "Push-To-Talk Keys ---")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                   "Push [F] for Area Chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                   "Push [V] for Team Chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                              "Press both to speak in both -")));
#endif
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                                                           "")));
        Instructions.Add(InstructionText(VivoxH2Font, FText::FromString(                                        "Team Roster Details ---")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                     "Displays Team color and Player names -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(                "Headset appears if Player in 2D Team Chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString("Color panel lights up team color when speaking in 2D chat -")));
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(     "Color panel lights up green when speaking in 3D chat -")));
#if !(PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
        Instructions.Add(InstructionText(VivoxNormalFont, FText::FromString(           "Displays last color when in neither/both chats -")));
#endif

        FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), VivoxNormalFont, HUDLight);
        TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
        TextItem.FontRenderInfo = ShadowedFont;
        TextItem.EnableShadow(FLinearColor::Black);

        FVector2D TextSize;
        Canvas->StrLen(VivoxNormalFont, "W", TextSize.X, TextSize.Y);
        float EmptyLineSizeY = TextSize.Y; // Height of a "W" in VivoxNormalFont

        // start at bottom right corner
        FVector2D CurPos(TextOffsetX, TextOffsetY);
        Canvas->SetLinearDrawColor(FLinearColor::White);
        for (int i = Instructions.Num() - 1; i >= 0; --i) // print text from the bottom up
        {
            if (Instructions[i].Value.IsEmptyOrWhitespace())
            {
                CurPos.Y -= EmptyLineSizeY;
                continue;
            }
            TextItem.Font = Instructions[i].Key;
            TextItem.Text = Instructions[i].Value;
            Canvas->StrLen(TextItem.Font, TextItem.Text.ToString(), TextSize.X, TextSize.Y);
            Canvas->DrawItem(TextItem, CurPos.X - TextSize.X * ScaleUI, CurPos.Y - TextSize.Y * ScaleUI);
            CurPos.Y -= TextSize.Y * ScaleUI;
        }
    }
}

void AVivoxHUD::DrawVivoxRoster()
{
    if (!GetGameplayMode().Equals(TEXT("TDM"))) // Team Deathmatch
        return;

    AShooterGameState* const MyGameState = GetWorld()->GetGameState<AShooterGameState>();
    if (!MyGameState) return;

    AShooterPlayerController* MyPlayerController = Cast<AShooterPlayerController>(PlayerOwner);
    if (!MyPlayerController) return;

    AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(MyPlayerController->PlayerState);
    if (!MyPlayerState) return;

    int DebugInfoFactor = 5;
#if !UE_BUILD_SHIPPING
    if (GetNetMode() != NM_Standalone)
        DebugInfoFactor = 10;
#endif

    const float VivoxPosX = Canvas->OrgX + Offset * ScaleUI;
    const float VivoxPosY = Canvas->OrgY + DebugInfoFactor * Offset * ScaleUI;
    const float BoxPadding = 4.0f;

    const float NotSpeakingAlpha = 0.37f;
    const float TeamSpeakingAlpha = 0.74f;
    const float IconSpeakingAlpha = 1.00f;

    const int32 TeamRed = 0;
    const int32 TeamIndex = MyPlayerState->GetTeamNum();

    // create array of team-only players
    TArray<AShooterPlayerState*> TeamArray;
    for (int32 i = 0; i < MyGameState->PlayerArray.Num(); ++i)
    {
        AShooterPlayerState* CurPlayerState = Cast<AShooterPlayerState>(MyGameState->PlayerArray[i]);
        if (CurPlayerState && (CurPlayerState->GetTeamNum() == TeamIndex))
        {
            TeamArray.Add(CurPlayerState);
        }
    }

    // sort the array: local player < real players alphabetically < bots alphabetically
    TeamArray.Sort([this](const AShooterPlayerState& One, const AShooterPlayerState& Two) {
        if (One.GetShortPlayerName().Equals(GetGameInstance()->GetFirstGamePlayer()->GetNickname()))
            return true;
        else if ((One.bIsABot && Two.bIsABot) || (!One.bIsABot && !Two.bIsABot))
            return One.GetPlayerName().Compare(Two.GetPlayerName(), ESearchCase::IgnoreCase) < 0; // One < Two
        else
            return (bool)Two.bIsABot;
    });

    // get Vivox channel session (same for all participants)
    TSharedPtr<IChannelSession> ChannelSession = VivoxGameInstance->GetChannelSessionForRoster();

    // team roster
    for (int32 PlayerIndex = 0; PlayerIndex < TeamArray.Num(); ++PlayerIndex)
    {
        AShooterPlayerState* CurPlayerState = TeamArray[PlayerIndex];

        // origin position for this roster item
        FVector2D CurPos(VivoxPosX, VivoxPosY + PlayerIndex * (VivoxRosterBg.VL + BoxPadding) * ScaleUI);

        // background
        Canvas->SetLinearDrawColor(FLinearColor::White);
        Canvas->DrawIcon(VivoxRosterBg, CurPos.X, CurPos.Y, ScaleUI);

        bool bIsInAudio = false;
        bool bIsSpeaking = false;

        // If this player isn't a bot, check if in channel and speaking
        if (!CurPlayerState->bIsABot && ChannelSession.IsValid())
        {
            IParticipant * const *Participant = ChannelSession->Participants().Find(CurPlayerState->UniqueId.ToString());
            if (Participant)
            {
                bIsInAudio = true;
                if ((*Participant)->SpeechDetected())
                {
                    bIsSpeaking = true;
                }
            }
        }
        // @todo: make a more comprehensive Area chat speech detection UI; meanwhile, local player uses team panel for any speech indicator and others Team channel only
        if (CurPlayerState == MyPlayerState) {
            IParticipant * const *Participant = VivoxGameInstance->GetLoginSessionForRoster()->GetChannelSession(VivoxGameInstance->GetLastKnownTransmittingChannel()).Participants().Find(CurPlayerState->UniqueId.ToString());
            if (Participant && (*Participant)->SpeechDetected())
            {
                bIsSpeaking = true;
            }
        }

        // team color
        Canvas->SetLinearDrawColor(FLinearColor::White, bIsSpeaking ? TeamSpeakingAlpha : NotSpeakingAlpha);
        if ((CurPlayerState == MyPlayerState) && VivoxGameInstance->GetLastKnownTransmittingChannel().Type() == ChannelType::Positional)
        {
            Canvas->DrawIcon(VivoxRosterPositional, CurPos.X, CurPos.Y, ScaleUI);
        }
        else if (TeamIndex == TeamRed)
        {
            Canvas->DrawIcon(VivoxRosterRedTeam, CurPos.X, CurPos.Y, ScaleUI);
        }
        else // blue team
        {
            Canvas->DrawIcon(VivoxRosterBlueTeam, CurPos.X, CurPos.Y, ScaleUI);
        }

        // in-channel/speaking/muted icon
        if (bIsInAudio)
        {
            Canvas->SetLinearDrawColor(FLinearColor::White, bIsSpeaking ? IconSpeakingAlpha : NotSpeakingAlpha);
            if (CurPlayerState == MyPlayerState) // for local player, show if mic is muted.
                Canvas->DrawIcon(VivoxGameInstance->GetLoginSessionForRoster()->GetTransmissionMode() == TransmissionMode::None ? VivoxRosterMutedIcon : VivoxRosterChannelIcon, CurPos.X, CurPos.Y, ScaleUI);
            else
                Canvas->DrawIcon(VivoxRosterChannelIcon, CurPos.X, CurPos.Y, ScaleUI);
        }

        // name
        Canvas->SetLinearDrawColor(FLinearColor::White);
        float TextScale = 0.85f;
        float TextOffsetX = 80.0f;
        float TextOffsetY = 20.0f;
        FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), VivoxNormalFont, HUDLight);
        TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
        TextItem.FontRenderInfo = ShadowedFont;
        TextItem.EnableShadow(FLinearColor::Black);
        TextItem.Text = FText::FromString(CurPlayerState->GetShortPlayerName());
        Canvas->DrawItem(TextItem, CurPos.X + TextOffsetX * ScaleUI, CurPos.Y + TextOffsetY * ScaleUI);
    }
}

void AVivoxHUD::DrawVivoxPTTPanel() // Console only.
{
#if (PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
    const float BoxPadding = 2.0f;
    // origin X Y of first PTTPanel element
    float PTTPanelPosX = Canvas->OrgX + Offset * ScaleUI; /* origin X of first PTTPanel element */
    float PTTPanelPosY = Canvas->ClipY - (Offset + DeathMessagesBg.VL + 4.0) * ScaleUI;

    // origin position for this roster item
    FVector2D CurPos(PTTPanelPosX, PTTPanelPosY);

    // text setup
    FVector2D TextSize;
    float TextScale = 0.85f;
    FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), VivoxNormalFont, HUDLight);
    TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
    TextItem.FontRenderInfo = ShadowedFont;
    TextItem.EnableShadow(FLinearColor::Black);

    // area label
    TextItem.Text = FText::FromString("Area");
    Canvas->StrLen(TextItem.Font, TextItem.Text.ToString(), TextSize.X, TextSize.Y);
    CurPos.Y -= (TextSize.Y + BoxPadding) * ScaleUI;
    Canvas->SetLinearDrawColor(FLinearColor::White);
    Canvas->DrawItem(TextItem, CurPos.X + (VivoxPTTPanelBg.UL / 2 - TextSize.X * TextScale / 2) * ScaleUI, CurPos.Y);

    // background
    CurPos.Y -= (VivoxPTTPanelBg.VL + BoxPadding) * ScaleUI;
    Canvas->DrawIcon(VivoxPTTPanelBg, CurPos.X, CurPos.Y, ScaleUI);

    // active channel
    CurPos.Set(CurPos.X + 27 * ScaleUI, CurPos.Y + 38 * ScaleUI);
    if (VivoxGameInstance->GetLoginSessionForRoster()->GetTransmittingChannels().Num() == 0)
    {
        // not transmitting to specific channel; do nothing
    }
    else if (VivoxGameInstance->GetLastKnownTransmittingChannel().Type() == ChannelType::Positional) // 3D Area Chat Channel
    {
        Canvas->SetLinearDrawColor(FLinearColor::FromSRGBColor(FColor(0, 109, 101, 255))); // nice green color
        Canvas->DrawIcon(VivoxPTTPanelActive, CurPos.X, CurPos.Y, ScaleUI);

        // overlay
        Canvas->SetLinearDrawColor(FLinearColor(1, 1, 1, 0.5)); // White, 50% opacity
        Canvas->DrawIcon(VivoxPTTPanelOverlay, CurPos.X, CurPos.Y, ScaleUI);
    }
    else // Team Channel (or unimplemtented Echo if edited in)
    {
        FCanvasTileItem TileItem(CurPos, HUDVivoxTexture->Resource, FVector2D(VivoxPTTPanelActive.UL* ScaleUI, VivoxPTTPanelActive.VL* ScaleUI), GetTeamColor());
        MakeUV(VivoxPTTPanelActive, TileItem.UV0, TileItem.UV1, VivoxPTTPanelActive.U, VivoxPTTPanelActive.V, VivoxPTTPanelActive.UL, VivoxPTTPanelActive.VL);
        TileItem.Rotation = FRotator(0, 180, 0);
        TileItem.PivotPoint = FVector2D(0.5, 0);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);

        // overlay
        TileItem.SetColor(FLinearColor(1, 1, 1, 0.5)); // White, 50% opacity
        TileItem.Size = FVector2D(VivoxPTTPanelOverlay.UL * ScaleUI, VivoxPTTPanelOverlay.VL * ScaleUI);
        MakeUV(VivoxPTTPanelOverlay, TileItem.UV0, TileItem.UV1, VivoxPTTPanelOverlay.U, VivoxPTTPanelOverlay.V, VivoxPTTPanelOverlay.UL, VivoxPTTPanelOverlay.VL);
        Canvas->DrawItem(TileItem);
    }
    CurPos.Set(CurPos.X - 27 * ScaleUI, CurPos.Y - 38 * ScaleUI);

    // team label
    TextItem.Text = FText::FromString("Team");
    Canvas->StrLen(TextItem.Font, TextItem.Text.ToString(), TextSize.X, TextSize.Y);
    CurPos.Y -= (TextSize.Y + BoxPadding) * ScaleUI;
    Canvas->SetLinearDrawColor(FLinearColor::White);
    Canvas->DrawItem(TextItem, CurPos.X + (VivoxPTTPanelBg.UL / 2 - TextSize.X * TextScale / 2) * ScaleUI, CurPos.Y);
#endif
}

void AVivoxHUD::DrawHUD()
{
    Super::DrawHUD();

    // Render Vivox HUD elements - methods internally determine when they should draw.
    DrawVivoxRoster();
    DrawVivoxInfoText();
    DrawVivoxPTTPanel();
}
