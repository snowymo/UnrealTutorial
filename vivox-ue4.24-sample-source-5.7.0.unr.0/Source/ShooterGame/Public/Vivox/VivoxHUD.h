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

#include "UI/ShooterHUD.h"
#include "Vivox/VivoxGameInstance.h"
#include "Map.h"
#include "VivoxHUD.generated.h"

UCLASS()
class AVivoxHUD : public AShooterHUD
{
    GENERATED_UCLASS_BODY()

public:
    /** Main HUD update loop. */
    virtual void DrawHUD() override;

protected:
    /** Background frame for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterBg;

    /** Red team panel for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterRedTeam;

    /** Blue team panel for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterBlueTeam;

    /** Positional panel for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterPositional;

    /** In-Channel indicator for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterChannelIcon;

    /** Muted indicator for roster. */
    UPROPERTY()
    FCanvasIcon VivoxRosterMutedIcon;

    /** Background frame for PTT panel (console only). */
    UPROPERTY()
    FCanvasIcon VivoxPTTPanelBg;

    /** Active selection for PTT panel (console only). */
    UPROPERTY()
    FCanvasIcon VivoxPTTPanelActive;

    /** Frame overlay for PTT panel (console only). */
    UPROPERTY()
    FCanvasIcon VivoxPTTPanelOverlay;


    /** Texture for Vivox HUD elements */
    UPROPERTY()
    UTexture2D* HUDVivoxTexture;

    /** Heading 1 font - used for Vivox Instruction text. */
    UPROPERTY()
    UFont* VivoxH1Font;

    /** Heading 2 font -used for Vivox Instruction text. */
    UPROPERTY()
    UFont* VivoxH2Font;

    /** Normal font - used for Vivox Instruction and Roster text. */
    UPROPERTY()
    UFont* VivoxNormalFont;

    /** Stores the Online Session's SETTING_GAMEMODE. */
    FString CachedGameplayMode;

    /** Returns CachedGameplayMode, first setting if unset. */
    FString GetGameplayMode();

    /** Stores the size of the Debug Info box. */
    FVector2D CachedDebugInfoBoxSize;

    /** Returns CachedDebugInfoBoxSize, first setting if unset. */
    FVector2D GetDebugInfoBoxSize();

    /** Stores team color to use for PTT Panel (console only). */
    FLinearColor CachedTeamColor;

    /** Returns CachedTeamColor, first setting if unset. */
    FLinearColor GetTeamColor();

    /** Stores the Player State and speaking status of all teammates. */
    TMap<FString, bool> TeammatesInAudio;

    /** Draws Vivox informational text. */
    void DrawVivoxInfoText();

    /** Draws Vivox team roster. */
    void DrawVivoxRoster();

    /** Draws Vivox push-to-talk panel (console only). */
    void DrawVivoxPTTPanel();

    /** Called every time game is started. */
    virtual void PostInitializeComponents() override;

private:
    UVivoxGameInstance *VivoxGameInstance;
};
