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
#include "Vivox/VivoxPlayerController.h"
#include "Vivox/VivoxGameInstance.h"
#include "Vivox/VivoxTracer.h"

AVivoxPlayerController::AVivoxPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AVivoxPlayerController::PushToTalkAreaChannelPressed()
{
    Tracer::MinorMethodPrologue();
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanPushToTalk(PTTKey::PTTAreaChannel, true);
}

void AVivoxPlayerController::PushToTalkAreaChannelReleased()
{
    Tracer::MinorMethodPrologue();
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanPushToTalk(PTTKey::PTTAreaChannel, false);
}

void AVivoxPlayerController::PushToTalkTeamChannelPressed()
{
    Tracer::MinorMethodPrologue();
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanPushToTalk(PTTKey::PTTTeamChannel, true);
}

void AVivoxPlayerController::PushToTalkTeamChannelReleased()
{
    Tracer::MinorMethodPrologue();
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanPushToTalk(PTTKey::PTTTeamChannel, false);
}

void AVivoxPlayerController::PushToToggleAreaChannel()
{
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanToggleChat(PTTKey::PTTAreaChannel);
}

void AVivoxPlayerController::PushToToggleTeamChannel()
{
    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->MultiChanToggleChat(PTTKey::PTTTeamChannel);
}

void AVivoxPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    UVivoxGameInstance* VivoxGameInstance = GetWorld() != NULL ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);
    VivoxGameInstance->Update3DPosition(GetPawnOrSpectator()); // Track player in either Warmup or Match.
}

void AVivoxPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // unbind native voice and text
    int i = 0;
    while (i < InputComponent->GetNumActionBindings())
    {
        FInputActionBinding action = InputComponent->GetActionBinding(i);
        if (0 == action.GetActionName().ToString().Compare("PushToTalk") || 0 == action.GetActionName().ToString().Compare("ToggleChat"))
        {
            InputComponent->RemoveActionBinding(i);
        }
        else
        {
            ++i;
        }
    }

    // vivox multi-channel push-to-talk
    InputComponent->BindAction("PushToTalkAreaChannel", IE_Pressed, this, &AVivoxPlayerController::PushToTalkAreaChannelPressed);
    InputComponent->BindAction("PushToTalkAreaChannel", IE_Released, this, &AVivoxPlayerController::PushToTalkAreaChannelReleased);
    InputComponent->BindAction("PushToTalkTeamChannel", IE_Pressed, this, &AVivoxPlayerController::PushToTalkTeamChannelPressed);
    InputComponent->BindAction("PushToTalkTeamChannel", IE_Released, this, &AVivoxPlayerController::PushToTalkTeamChannelReleased);

#if (PLATFORM_PS4 || PLATFORM_XBOXONE || PLATFORM_SWITCH)
    // key bindings for for push-to-toggle chat on consoles
    InputComponent->BindAction("PushToToggleAreaChannel", IE_Pressed, this, &AVivoxPlayerController::PushToToggleAreaChannel);
    InputComponent->BindAction("PushToToggleTeamChannel", IE_Pressed, this, &AVivoxPlayerController::PushToToggleTeamChannel);
#endif
}

void AVivoxPlayerController::ClientJoinVoice_Implementation(const FString &GameMode, const FString &OnlineSessionId, const int32 &TeamNum)
{
    Tracer::MajorMethodPrologue("%s %s %d", *GameMode, *OnlineSessionId, TeamNum);

    if (!IsPrimaryPlayer())
        return;

    UVivoxGameInstance *VivoxGameInstance = GetWorld() ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);

    VivoxGameInstance->JoinVoiceChannels(GameMode, OnlineSessionId, TeamNum);
}

void AVivoxPlayerController::ClientReturnToMainMenu_Implementation(const FString& ReturnReason)
{
    Tracer::MajorMethodPrologue("%s", *ReturnReason);

    if (!IsPrimaryPlayer())
        return;

    UVivoxGameInstance *VivoxGameInstance = GetWorld() ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);

    VivoxGameInstance->LeaveVoiceChannels();

    Super::ClientReturnToMainMenu_Implementation(ReturnReason);
}

void AVivoxPlayerController::HandleReturnToMainMenu()
{
    Tracer::MinorMethodPrologue();

    if (!IsPrimaryPlayer())
        return;

    UVivoxGameInstance *VivoxGameInstance = GetWorld() ? CastChecked<UVivoxGameInstance>(GetWorld()->GetGameInstance()) : NULL;
    CHECKRET(VivoxGameInstance);

    VivoxGameInstance->LeaveVoiceChannels();

    Super::HandleReturnToMainMenu();
}
