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
#include "VivoxCoreCommon.h"

/**
 * \brief Common properties representing a player in a channel
 */
class VIVOXCORE_API IParticipantProperties
{
public:
    /* @cond */
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginSetIsMutedForAllCompleted, VivoxCoreError)
    /* @endcond */
public:
    virtual ~IParticipantProperties() = default;
    /**
     * \brief True if this participant corresponds to the currently connected user.
     */
    virtual bool IsSelf() const = 0;
    /**
     * \brief If true, the user is in audio
     */
    virtual bool InAudio() const = 0;
    /**
     * \brief If true, the user is in text
     */
    virtual bool InText() const = 0;
    /**
     * \brief If true, the user is speaking
     */
    virtual bool SpeechDetected() const = 0;
    /**
     * \brief The audio energy for this user. This can be used to build a VU meter for this user.
     */
    virtual double AudioEnergy() const = 0;
    /**
     * \brief The gain for this user, only for the currently logged in user.
     *  The valid range is between -50 and 50.
     *  Positive values increase volume, negative values decrease volume.
     *  Zero (default) leaves volume unchanged.
     */
    virtual int LocalVolumeAdjustment() const = 0;
    /**
    * \brief Set the gain for this user, only for the currently logged in user.
    *  The valid range is between -50 and 50.
    *  Positive values increase volume, negative values decrease volume.
    *  Zero (default) leaves volume unchanged.
    */
    virtual VivoxCoreError SetLocalVolumeAdjustment(int value) = 0;

    /**
     * \brief Returns the muted status of the participant for the currently connected user only.
     */
    virtual bool LocalMute() const = 0;
    /**
    * \brief Use this to silence or un-silence this participant for the currently connected user only.
    */
    virtual void SetLocalMute(bool value) = 0;

    /**
     * \brief Indicates if the user is currently typing.
     */
    virtual bool IsTyping() const = 0;

    /**
     * \brief Indicates if the user has been muted for all users.
     */
    virtual bool IsMutedForAll() const = 0;
    /**
    * \brief The delegate called when BeginSetIsMutedForAll() completes
    */
    typedef FDelegateBeginSetIsMutedForAllCompleted::FDelegate FOnBeginSetIsMutedForAllCompletedDelegate;
    /**
    * \brief Use this to silence or un-silence this participant for everyone in the channel.
    *
    * \param setMuted true to mute or false to unmute
    * \param accessToken the access token granting the user permission to mute this participant in the channel
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidArgument if accessToken is empty
    * \remarks Developers of games that do not have secure communications requirements can use GetMuteForAllToken() to generate the required access token.
    */
    virtual VivoxCoreError BeginSetIsMutedForAll(bool setMuted, const FString& accessToken, FOnBeginSetIsMutedForAllCompletedDelegate theDelegate = FOnBeginSetIsMutedForAllCompletedDelegate()) const = 0;
    /**
    * \brief Get a token that can be used to silence or un-silence a participant for everyone in a channel.
    *
    * \param tokenSigningKey the key corresponding to the issuer for this account that is used to sign the token.
    * \param tokenExpirationDuration the length of time the token is valid for.
    * \return A token that can be used to mute a participant in a channel.
    * \remarks To be used only by applications without secure communications requirements.
    */
    virtual FString GetMuteForAllToken(const FString & tokenSigningKey, FTimespan tokenExpirationDuration) const = 0;
};
