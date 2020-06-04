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

#pragma once

#include "Player/ShooterPlayerController.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "VivoxPlayerController.generated.h"

#define CHECKRET(cond) if (!(cond)) { UE_LOG(LogTemp, Error, TEXT("CHECKRET(%s) Failed"), TEXT(#cond)); return; }

UCLASS(config=Game)
class AVivoxPlayerController : public AShooterPlayerController
{
    GENERATED_UCLASS_BODY()

public:
    void PushToTalkAreaChannelPressed();
    void PushToTalkAreaChannelReleased();
    void PushToTalkTeamChannelPressed();
    void PushToTalkTeamChannelReleased();

    void PushToToggleAreaChannel();
    void PushToToggleTeamChannel();

    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    virtual void HandleReturnToMainMenu() override;
    void ClientReturnToMainMenu_Implementation(const FString& ReturnReason) override;

    UFUNCTION(reliable, client)
    void ClientJoinVoice(const FString &GameMode, const FString &OnlineSessionId, const int32 &TeamNum = -1);
};
