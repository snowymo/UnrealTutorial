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
#include "IParticipantProperties.h"

/**
 * \brief A participant in a channel
 *
 * \remarks
 * Note that the key for this interface is an account name returned by AccountId::Name()
 */
class VIVOXCORE_API IParticipant : public IParticipantProperties
{
public:
    /**
     * \brief Identifies the participant's specific login location
     * \remarks This is not the same as Account, but is the same as IPresenceLocation::LocationId(). Rarely used.
     * Accounts can't be connected to a ChannelSession from more than one location at a time (last in wins),
     * so the Account() of each Participant is already unique, but this will match a particular Presence Location
     * when a user is signed in from more than one, which may be useful information for some implementations.
     */
    virtual const FString &ParticipantId() const = 0;

    /**
     * \brief The ChannelSession that owns this Participant
     */
    virtual IChannelSession &ParentChannelSession() const = 0;

    /**
     * \brief The AccountId associated with this Participant
     */
    virtual const AccountId &Account() const = 0;
};
