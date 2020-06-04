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
#include "Containers/UnrealString.h"
#include "Misc/Optional.h"
/**
 * The unique identitier for a player that accesses a Vivox instance
 */
class VIVOXCORE_API AccountId
{
    FString _domain;
    FString _name;
    FString _issuer;
    FString _displayName;
    TArray<FString> _spokenLanguages;

public:
    /**
     * \brief Create an AccountId from a URI - Internal Use Only.
     * \param uri the uri of the account
     * \param displayName Optional display name that will be seen by others. Default is empty string
     * \return the AccountId
     */
    static AccountId CreateFromUri(const FString &uri, const TOptional<FString> &displayName = TOptional<FString>());
    /**
     * \brief Shortcut for AccountId::CreateFromUri(uri).Name() - Internal Use Only.
     * \param uri the uri of the account
     * \return the Name returned from an AccountId created with uri
     */
    static FString AccountNameFromUri(const FString &uri);
    /**
     * \brief Default Constructor
     */
    AccountId();
    /**
     * \brief Constructor
     * \param issuer The issuer that is responsible for authenticating this account
     * \param name The unique account name you assigned to the player
     * \param domain The Vivox domain that provides service for this account, for example vdx5.vivox.com
     * \param displayName Optional display name that will be seen by others. Default is empty string
     * \param spokenLanguages Optional array of languages used as hints for audio transcription. Default is empty array, implies "en"
     * \remarks The combined length of 'Issuer' + 'Name' must not exceed 124 characters. The DisplayName must not exceed 63 characters.
     * \remarks Name and DisplayName must be chosen only from letters a-z and A-Z, numbers 0-9, and the special characters: =+-_.!~()%
     * \remarks Up to 3 spoken languages may be specified in order of preference to inform transcription of all users in transcribed channels.
     * \remarks IETF language tag strings are not validated, but are expected to conform to BCP-47 (https://tools.ietf.org/html/bcp47).
     */
    AccountId(const FString &issuer, const FString &name, const FString &domain, const TOptional<FString> &displayName = TOptional<FString>(), const TOptional<TArray<FString>>& spokenLanguages = TOptional<TArray<FString>>());
    /**
     * \brief Destructor
     */
    ~AccountId();

    /**
     * \brief The issuer that is responsible for authenticating this account
     */
    const FString &Issuer() const;
    /**
     * \brief The unique account name you assigned to the player
     * \remarks This is the value the SDK uses internally to identify the user. Typically matches the game or online id of the player, or a hash of it to ensure character requirements.
     */
    const FString &Name() const;
    /**
     * \brief The Vivox domain that provides service for this account
     */
    const FString &Domain() const;
    /**
    * \brief Optional display name that will be seen by others. Default is empty string
    * \remarks The Vivox SDK makes no use of this value internally. You may use it in your game however you wish, for example, in rosters or buddy lists. It is not required to be unique.
    */
    const FString &DisplayName() const;
    /**
    * \brief Optional array of languages used as hints for audio transcription. Default is empty array, implies "en"
    * \remarks Up to 3 spoken languages used in order of preference to inform transcription of all users in transcribed channels.
    *  IETF language tag strings are not validated, but are expected to conform to BCP-47 (https://tools.ietf.org/html/bcp47).
    */
    const TArray<FString> &SpokenLanguages() const;

    /**
     * \brief True if Issuer, Name, and Domain are all empty
     */
    bool IsEmpty() const;

    /**
     * \brief True if Issuer and Domain are non-empty, and Name meets restrictions
     */
    bool IsValid() const;

    /**
     * \brief Internal use only
     */
    FString ToString() const;

    /** \brief Equality operator */
    bool operator ==(const AccountId &RHS) const;
    /** \brief InEquality operator */
    bool operator !=(const AccountId &RHS) const { return !operator ==(RHS); }

    /**
     * \brief True if id is null or empty
     */
    static bool IsNullOrEmpty(AccountId *id);
};

/**
 * \brief Standard Unreal Engine Hash Function that allows AccountId to be used as a key in UE4 collections.
 * \param id the account id
 * \return the hash
 */
uint32 VIVOXCORE_API GetTypeHash(const AccountId &id);
