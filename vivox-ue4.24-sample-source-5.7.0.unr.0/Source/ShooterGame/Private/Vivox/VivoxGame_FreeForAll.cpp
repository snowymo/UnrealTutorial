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
#include "Vivox/VivoxGame_FreeForAll.h"
#include "Vivox/VivoxHUD.h"
#include "Vivox/VivoxPlayerController.h"

AVivoxGame_FreeForAll::AVivoxGame_FreeForAll(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    HUDClass = AVivoxHUD::StaticClass();
    PlayerControllerClass = AVivoxPlayerController::StaticClass();
}

void AVivoxGame_FreeForAll::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    FString GameMode;
    FString OnlineSessionId;

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

    // Needs to be called after the parent constructor is called, otherwise the player's team will not have been set yet.
    VivoxPlayerController->ClientJoinVoice(GameMode, OnlineSessionId);
}
