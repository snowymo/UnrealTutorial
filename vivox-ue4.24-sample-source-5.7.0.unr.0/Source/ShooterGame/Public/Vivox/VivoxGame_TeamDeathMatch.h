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

#include "Online/ShooterGame_TeamDeathMatch.h"
#include "VivoxGame_TeamDeathMatch.generated.h"

UCLASS()
class AVivoxGame_TeamDeathMatch : public AShooterGame_TeamDeathMatch
{
    GENERATED_UCLASS_BODY()

    void PostLogin(APlayerController* NewPlayer) override;

    /**
     * Handles all player initialization that is shared between the travel methods
     * (i.e. called from both PostLogin() and HandleSeamlessTravelPlayer())
     */
    void GenericPlayerInitialization(AController* C) override;
};
