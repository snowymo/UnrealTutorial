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
#include "Online/ShooterGameSession.h"
#include "Online/ShooterPlayerState.h"
#include "Vivox/VivoxGame_TeamDeathMatch.h"
#include "Vivox/VivoxHUD.h"
#include "Vivox/VivoxPlayerController.h"
#include "Matinee/MatineeActor.h"

AVivoxGame_TeamDeathMatch::AVivoxGame_TeamDeathMatch(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    HUDClass = AVivoxHUD::StaticClass();
    PlayerControllerClass = AVivoxPlayerController::StaticClass();
}

void AVivoxGame_TeamDeathMatch::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    FString GameMode;
    FString OnlineSessionId;
    int32 TeamNum;

    AVivoxPlayerController *VivoxPlayerController = CastChecked<AVivoxPlayerController>(NewPlayer);
    CHECKRET(VivoxPlayerController);

    IOnlineSubsystem *OnlineSubsystem = IOnlineSubsystem::Get();
    CHECKRET(OnlineSubsystem);
    IOnlineSessionPtr SessionSubsystem = OnlineSubsystem->GetSessionInterface();
    CHECKRET(SessionSubsystem.IsValid());
    FNamedOnlineSession *Session = SessionSubsystem->GetNamedSession(NAME_GameSession);
    CHECKRET(Session);
    Session->SessionSettings.Get(SETTING_GAMEMODE, GameMode);
    OnlineSessionId = Session->SessionInfo->GetSessionId().ToString();

    AShooterPlayerState *ShooterPlayerState = CastChecked<AShooterPlayerState>(VivoxPlayerController->PlayerState);
    CHECKRET(ShooterPlayerState);
    TeamNum = ShooterPlayerState->GetTeamNum();

    // Needs to be called after the parent constructor is called, otherwise the player's team will not have been set yet.
    VivoxPlayerController->ClientJoinVoice(GameMode, OnlineSessionId, TeamNum);
}

/**
 * \brief This override prevents the engine's default voice implementation from sending networked voice traffic by omitting a few lines from the original method.
 * \param C The player being initialized.
 */
void AVivoxGame_TeamDeathMatch::GenericPlayerInitialization(AController* C)
{
    APlayerController* PC = Cast<APlayerController>(C);
    if (PC != nullptr)
    {
        InitializeHUDForPlayer(PC);

        ReplicateStreamingStatus(PC);

        bool HidePlayer = false, HideHUD = false, DisableMovement = false, DisableTurning = false;

        // Check to see if we should start in cinematic mode (matinee movie capture)
        if (ShouldStartInCinematicMode(PC, HidePlayer, HideHUD, DisableMovement, DisableTurning))
        {
            PC->SetCinematicMode(true, HidePlayer, HideHUD, DisableMovement, DisableTurning);
        }

        // Add the player to any matinees running so that it gets in on any cinematics already running, etc
        TArray<AMatineeActor*> AllMatineeActors;
        GetWorld()->GetMatineeActors(AllMatineeActors);
        for (int32 i = 0; i < AllMatineeActors.Num(); i++)
        {
            AllMatineeActors[i]->AddPlayerToDirectorTracks(PC);
        }
    }
}
