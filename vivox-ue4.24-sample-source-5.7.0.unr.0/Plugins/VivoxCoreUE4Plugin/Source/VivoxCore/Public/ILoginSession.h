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
#include "Engine/Blueprint.h"
#include "VivoxCoreCommon.h"
#include "ITextToSpeech.h"

/**
 * \brief The state of the login session
 */
UENUM(BlueprintType)
enum class LoginState : uint8
{
    /**
     * \brief Login Session is logged out
     */
    LoggedOut,
    /**
     * \brief Login Session is in the process of logging in
     */
    LoggingIn,
    /**
     * \brief Login Session is logged in
     */
    LoggedIn,
    /**
     * \brief Login Session is in the process of logging out.
     */
    LoggingOut
};

/**
 * \brief How to handle incoming subscriptions (friend/follower requests)
 */
UENUM(BlueprintType)
enum class SubscriptionMode : uint8
{
    /**
     * \brief Automatically accept all incoming subscription requests.
     */
    Accept,
    /**
     * \brief Automatically block all incoming subscription requests.
     */
    Block,
    /**
     * \brief Defer incoming subscription request handling to the application.
     * This will result in EventSubscriptionRequestReceived firing
     */
    Defer
};

UENUM(BlueprintType)
enum class SubscriptionReply : uint8
{
    /**
     * \brief Allow the incoming subscription
     */
    Allow,
    /**
     * \brief Block the incoming subscription
     */
    Block
};

/**
 * \brief Defines the policy of where microphone audio and injected audio get broadcast.
 */
UENUM(BlueprintType)
enum class TransmissionMode : uint8
{
    /**
     * \brief Adopts a policy of transmission into no channels.
     */
    None,
    /**
     * \brief Adopts a policy of transmission into one channel at a time.
     */
    Single,
    /**
     * \brief Adopts a policy of transmission into all channels at once.
     */
    All
};

/**
 * \brief Defines how often the SDK raises <b>speech related</b> IChannelSession::EventAfterParticipantUpdated events while in channel.
 * \remarks Use a per second rate (at the fidelity of your choice) when implementing a real-time audio energy meter.
 * This controls participant update events due to changes in speech detection or changes in audio energy, only.
 * Other participant property state change events (active media, muted state, etc.) will occur regardless of setting.
 */
UENUM(BlueprintType)
enum class ParticipantSpeakingUpdateRate : uint8
{
    /**
     * \brief On participant state change in speech detection only (excludes audio energy).
     */
    StateChange,
    /**
     * \brief Don't raise events for changes in participant speech detection or audio energy (use with caution).
     */
    Never,
    /**
     * \brief Update participant properties up to 1 time per second (includes audio energy).
     */
    Update1Hz,
    /**
     * \brief Update participant properties up to 5 times per second (includes audio energy).
     */
    Update5Hz,
    /**
     * \brief Update participant properties up to 10 times per second (includes audio energy).
     */
    Update10Hz
};

/**
 * \brief An instance of an account logging into the Vivox Network
 */
class VIVOXCORE_API ILoginSession
{
public:
    /* @cond */
    DECLARE_EVENT_OneParam(ILoginSession, SubscriptionRequestReceived, const AccountId &)
    DECLARE_EVENT_OneParam(ILoginSession, DirectedTextMessageReceived, const IDirectedTextMessage &)
    DECLARE_EVENT_TwoParams(ILoginSession, SendDirectedTextMessageFailed, VivoxCoreError, const FString &)
    DECLARE_EVENT_OneParam(ILoginSession, StateChanged, LoginState)
    DECLARE_EVENT(ILoginSession, AudioInjectionCompleted)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginAddBlockedSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginRemoveBlockedSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginAddAllowedSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginRemoveAllowedSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginAddPresenceSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginRemovePresenceSubscriptionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginSendSubscriptionReplyCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateBeginSendDirectedMessageCompleted, VivoxCoreError, const FString &)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginStartAudioInjectionCompleted, VivoxCoreError)
    DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateBeginLoginCompleted, VivoxCoreError)
    /* @endcond */
public:
    /**
     * \brief destructor
     */
    virtual ~ILoginSession() = default;
    /**
     * \brief The list of channel sessions associated with this login session
     */
    virtual const TMap<ChannelId, TSharedPtr<IChannelSession> > &ChannelSessions() const = 0;

    /**
     * \brief The list of presence subscriptions associated with this login session
     * \remarks This typically corresponds to a list of "friends" or "followers"
     */
    virtual const TMap <AccountId, TSharedPtr<IPresenceSubscription> > &PresenceSubscriptions() const = 0;

    /**
     * \brief The list of accounts blocked from seeing this accounts online status
     */
    virtual const TSet<AccountId> &BlockedSubscriptions() const = 0;

    /**
     * \brief The list of accounts allowed to see this accounts online status
     */
    virtual const TSet<AccountId> &AllowedSubscriptions() const = 0;

    /**
    * \brief An event indicating that a new subscription request has been received
    * \remarks That is, another player would like to be your buddy.
    */
    SubscriptionRequestReceived EventSubscriptionRequestReceived;
    /**
     * \brief An event indicating that another player has sent you a text message
     */
    DirectedTextMessageReceived EventDirectedTextMessageReceived;

    /**
     * \brief The current state of this login session
     */
    virtual LoginState State() const = 0;
    /**
    * \brief The event fired when State() changes, except on player initiated logouts.
    * \remarks Loss of network connectivity will cause this event to fire
    */
    StateChanged EventStateChanged;

    /**
     * \brief The online status that is sent to those accounts subscribing to the presence of this account
     */
    virtual const Presence &CurrentPresence() = 0;
    /**
    * \brief Set this player's online status
    */
    virtual VivoxCoreError SetCurrentPresence(const Presence &value) = 0;

    /**
     * \brief The unique identifier for this LoginSession
     */
    virtual const AccountId &LoginSessionId() const = 0;

    /**
    * \brief The delegate called when BeginLogin() completes.
    */
    typedef FDelegateBeginLoginCompleted::FDelegate FOnBeginLoginCompletedDelegate;

    /**
    * \brief Begin logging in this session, with presence
    *
    * \param server The URI of the Vivox instance assigned to you
    * \param accessToken an access token provided by your game server that enables this login
    * \param subscriptionMode how to handle incoming subscriptions (friend/follower requests)
    * \param presenceSubscriptions A list of accounts for which this user wishes to monitor online status
    * \param blockedPresenceSubscriptions A list of accounts that are not allowed to see this user's online status
    * \param allowedPresenceSubscriptions A list of accounts that are allowed to see this user's online status
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    * \remarks
    * Developer of games that do not have secure communications requirements can use GetLoginToken to generate the required access token.
    */
    virtual VivoxCoreError BeginLogin(
        const FString &server,
        const FString & accessToken,
        SubscriptionMode subscriptionMode,
        const TSet<AccountId> & presenceSubscriptions,
        const TSet<AccountId> & blockedPresenceSubscriptions,
        const TSet<AccountId> & allowedPresenceSubscriptions,
        FOnBeginLoginCompletedDelegate theDelegate = FOnBeginLoginCompletedDelegate()) = 0;

    /**
     * \brief Begin logging in this session, with no presence
      *
     * \param server The URI of the Vivox instance assigned to you
     * \param accessToken an access token provided by your game server that enables this login
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     * \remarks
     * Developer of games that do not have secure communications requirements can use GetLoginToken to generate the required access token.
     */
    virtual VivoxCoreError BeginLogin(
        const FString &server,
        const FString & accessToken,
        FOnBeginLoginCompletedDelegate theDelegate = FOnBeginLoginCompletedDelegate()) = 0;

    /**
     * \brief Gets the channel session for this channelId, creating one if necessary
     *
     * \param channelId the id of the channel
     * \return the channel session
     */
    virtual IChannelSession &GetChannelSession(const ChannelId &channelId) = 0;

    /**
     * \brief Deletes the channel session for this channelId, disconnecting the session if necessary
     *
     * \param channelId the id of the channel
     */
    virtual void DeleteChannelSession(const ChannelId &channelId) = 0;

    /**
     * \brief The delegate to be called when BeginAddBlockedSubscription completes
     */
    typedef FDelegateBeginAddBlockedSubscriptionCompleted::FDelegate FOnBeginAddBlockedSubscriptionCompletedDelegate;

    /**
     * \brief Block incoming subscription requests from the specified account
     *
     * \param accountId the account id to block
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    virtual VivoxCoreError BeginAddBlockedSubscription(const AccountId &accountId, FOnBeginAddBlockedSubscriptionCompletedDelegate theDelegate = FOnBeginAddBlockedSubscriptionCompletedDelegate()) = 0;

    /**
     * \brief The delegate to be called when BeginRemoveBlockedSubscription completes
     */
    typedef FDelegateBeginRemoveBlockedSubscriptionCompleted::FDelegate FOnBeginRemoveBlockedSubscriptionCompletedDelegate;


    /**
     * \brief Unblock incoming subscription requests from the specified account. Subscription requests from the specified account will cause an event to be raised to the application.
     *
     * \param accountId the account id to unblock
     * \param theDelegate a delegate to call when this operation completes
     * @return 0 on success
     */
    virtual VivoxCoreError  BeginRemoveBlockedSubscription(const AccountId & accountId, FOnBeginRemoveBlockedSubscriptionCompletedDelegate theDelegate = FOnBeginRemoveBlockedSubscriptionCompletedDelegate()) = 0;

    /**
     * \brief The delegate to be called when BeginAddAllowedSubscription completes
     */
    typedef FDelegateBeginAddAllowedSubscriptionCompleted::FDelegate FOnBeginAddAllowedSubscriptionCompletedDelegate;

    /**
     * \brief Allow incoming subscription requests from the specified account
     *
     * \param accountId the account id to allow
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    virtual VivoxCoreError  BeginAddAllowedSubscription(const AccountId & accountId, FOnBeginAddAllowedSubscriptionCompletedDelegate theDelegate = FOnBeginAddAllowedSubscriptionCompletedDelegate()) = 0;

    /**
     * \brief The delegate to be called when BeginRemoveAllowedSubscription completes
     */
    typedef FDelegateBeginRemoveAllowedSubscriptionCompleted::FDelegate FOnBeginRemoveAllowedSubscriptionCompletedDelegate;

    /**
     * \brief Disallow automatic acceptance of incoming subscription requests from the specified account. Subscription requests from the specified account will cause an event to be raised to the application.
     *
     * \param accountId the account id to disallow
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    virtual VivoxCoreError  BeginRemoveAllowedSubscription(const AccountId & accountId, FOnBeginRemoveAllowedSubscriptionCompletedDelegate theDelegate = FOnBeginRemoveAllowedSubscriptionCompletedDelegate()) = 0;

    /**
     * \brief The delegate to be called when BeginAddPresenceSubscription completes
     */
    typedef FDelegateBeginAddPresenceSubscriptionCompleted::FDelegate FOnBeginAddPresenceSubscriptionCompletedDelegate;

    /**
    * \brief Subscribe to the specified account
    *
    * \param accountId the account id to subscribe to
    * \param theDelegate a delegate to call when this operation completes
    * \remarks This method will automatically allow accountId to see the subscriber's online status
    * \return 0 on success
    */
    virtual VivoxCoreError  BeginAddPresenceSubscription(const AccountId & accountId, FOnBeginAddPresenceSubscriptionCompletedDelegate theDelegate = FOnBeginAddPresenceSubscriptionCompletedDelegate()) = 0;
    /**
    * \brief The delegate to be called when BeginRemovePresenceSubscription completes
    */
    typedef FDelegateBeginRemovePresenceSubscriptionCompleted::FDelegate FOnBeginRemovePresenceSubscriptionCompletedDelegate;
    /**
    * \brief Unsubscribe from the specified account
    *
    * \param accountId the account id to subscribe to
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    */
    virtual VivoxCoreError BeginRemovePresenceSubscription(const AccountId & accountId, FOnBeginRemovePresenceSubscriptionCompletedDelegate theDelegate = FOnBeginRemovePresenceSubscriptionCompletedDelegate()) = 0;

    /**
    * \brief The delegate to be called when BeginSendSubscriptionReply completes
    */
    typedef FDelegateBeginSendSubscriptionReplyCompleted::FDelegate FOnBeginSendSubscriptionReplyCompletedDelegate;

    /**
    * \brief Reply to a subscription request received via EventSubscriptionRequestReceived
    *
    * \param accountId the accountId to reply to
    * \param replyType the SubscriptionReply type to use: Allow or Block
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    */
    virtual VivoxCoreError BeginSendSubscriptionReply(const AccountId & accountId, const SubscriptionReply & replyType, FOnBeginSendSubscriptionReplyCompletedDelegate theDelgate = FOnBeginSendSubscriptionReplyCompletedDelegate()) = 0;
    /**
     * \brief The delegate to be called when BeginSendDirectedMessage completes
     *
     * \remarks The VivoxCoreError parameter is a status code for the request and the FString parameter
     *  is a request ID unique to the directed message sent. In the event that the message can't be
     *  delivered to the intended recipient, EventSendDirectedTextMessageFailed will be raised with an
     *  error code and a matching request ID that can be compared with this one to determine which
     *  message failed to be delivered.
     */
    typedef FDelegateBeginSendDirectedMessageCompleted::FDelegate FOnBeginSendDirectedMessageCompletedDelegate;
    /**
    * \brief Send a message to the specific account
    *
    * \param accountId the intended recipient of the message
    * \param language the language of the message e.g "en". This can be null to use the default language ("en" for most systems). This must conform to BCP-47 (https://tools.ietf.org/html/bcp47)
    * \param message the body of the message
    * \param applicationStanzaNamespace an optional namespace element for additional application data
    * \param applicationStanzaBody the additional application data body
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidOperation if BeginLogin was called with no presence enabled (short form)
    */
    virtual VivoxCoreError BeginSendDirectedMessage(const AccountId & accountId, const FString &language, const FString & message, const FString & applicationStanzaNamespace, const FString & applicationStanzaBody, FOnBeginSendDirectedMessageCompletedDelegate theDelegate = FOnBeginSendDirectedMessageCompletedDelegate()) = 0;

    /**
    * \brief Send a message to the specific account
    *
    * \param accountId the intended recipient of the message
    * \param message the body of the message
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidOperation if BeginLogin was called with no presence enabled (short form)
    */
    virtual VivoxCoreError BeginSendDirectedMessage(const AccountId & accountId, const FString & message, FOnBeginSendDirectedMessageCompletedDelegate theDelegate = FOnBeginSendDirectedMessageCompletedDelegate()) = 0;

    /**
     * \brief An event indicating a directed message delivery failed.
     *
     * \remarks The VivoxCoreError parameter specifies the reason for failure and the FString parameter
     *  is a request ID matching the one returned in FOnBeginSendDirectedMessageCompletedDelegate when
     *  the message was sent. Match this request ID with the one returned in the delegate to determine
     *  which directed message failed to be delivered.
     */
    SendDirectedTextMessageFailed EventSendDirectedTextMessageFailed;

    /**
    * \brief The delegate to be called when BeginStartAudioInjection completes
    */
    typedef FDelegateBeginStartAudioInjectionCompleted::FDelegate FOnBeginStartAudioInjectionCompletedDelegate;
    /**
    * \brief Start broadcasting audio from a file to all connected ChannelSessions (see remarks)
    *
    * \param filePath the full pathname for the WAV file to play (MUST be single channel, 16-bit PCM)
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidState if not in Audio Connected ChannelSession
    *
    * \remarks Injected audio is treated like a second microphone you're speaking into. This
    *  means that muting or disconnecting your input device won't stop others from hearing
    *  the file audio, but muting yourself in a channel, stopping transmission to a channel,
    *  or disconnecting channel AudioState will. See Developer Guide for more details.
    */
    virtual VivoxCoreError BeginStartAudioInjection(const FString &filePath, FOnBeginStartAudioInjectionCompletedDelegate theDelegate = FOnBeginStartAudioInjectionCompletedDelegate()) = 0;

    /**
    * \brief Stop broadcasting audio from a file
    */
    virtual VivoxCoreError StopAudioInjection() = 0;

    /**
    * \brief True if currently broadcasting audio from a file
    */
    virtual bool IsAudioInjecting() = 0;

    /**
    * \brief An event indicating that you are no longer injecting audio
    * \remarks This event is raised either if injection is stopped early with StopAudioInjection()
    *  or the file you're injecting audio from has reached its natural end.
    */
    AudioInjectionCompleted EventAudioInjectionCompleted;

    /**
     * \brief Set whether microphone audio and injected audio should be transmitted to no channels, all channels, or a single specific channel.
     * \param mode enum specifying a transmission policy
     * \param singleChannel the specific channel to transmit to when TransmissionMode::Single is set (ignored otherwise)
     * \remarks Audio transmission changes take effect immediately and also affect future channel joins as follows:
     * - <b>None:</b> audio will not automatically transmit to new channels nor to text-only channels when audio is added.
     * - <b>All:</b> audio automatically transmits to new channels and to text-only channels when audio is added.
     * - <b>Single:</b> audio will transmit into the channel specified only, and will not automatically switch to new channels even if this channel is disconnected.
     * 
     * <b>Important:</b> You can override and change this setting by passing `true` for the switchTransmission argument in IChannelSession::BeginConnect() and IChannelSession::BeginSetAudioConnected().
     * \return VxErrorInvalidArgument if mode is TransmissionMode::Single and default/no ChannelId argument is provided
     */
    virtual VivoxCoreError SetTransmissionMode(TransmissionMode mode, ChannelId singleChannel = ChannelId()) = 0;

    /**
     * \brief Get the transmission policy set for the player.
     * \return the active TransmissionMode
     * \see SetTransmissionMode()
     */
    virtual TransmissionMode GetTransmissionMode() const = 0;

    /**
     * \brief Get the ChannelId of each channel that will broadcast non-muted microphone input and injected audio.
     * \return an array of transmitting ChannelIds
     * \see SetTransmissionMode()
     */
    virtual TArray<ChannelId> GetTransmittingChannels() const = 0;

    /**
     * \brief Set how often the SDK will raise IChannelSession::EventAfterParticipantUpdated events relating to speaking activity.
     * \param rate enum specifying the desired frequency of participant events due to changes in speech detection or audio energy
     * \remarks By default, the Vivox SDK is set up to support a simple speaking/not-speaking indicator for channel participants;
     * for this use case, keeping the default value of ParticipantSpeakingUpdateRate::StateChange is sufficient. If you'd like to
     * create a dynamic voice activity indicator or other real-time visual representation of audio energy levels, supply this 
     * function with a ParticipantSpeakingUpdateRate value of Update1Hz, Update5Hz, or Update10Hz for updates up to 1, 5, or 10
     * times per second as audio energy and other participant properties change. See the Unreal Developer Guide for more information.
     *
     * <b>Important:</b> While required for displaying real-time voice levels, setting a ParticipantSpeakingUpdateRate of any per 
     * second rate will necessarily increase network traffic between the application and Vivox servers.
     * \return 0 on success
     */
    virtual VivoxCoreError SetParticipantSpeakingUpdateRate(ParticipantSpeakingUpdateRate rate) = 0;

    /**
     * \brief Get the frequency set for speech related participant events for all participants in all connected channels.
     * \return the current ParticipantSpeakingUpdateRate
     * \see SetParticipantSpeakingUpdateRate()
     */
    virtual ParticipantSpeakingUpdateRate GetParticipantSpeakingUpdateRate() const = 0;

    /**
     * \brief The Text-To-Speech sub-system instance associated with this Login Session
     */
    virtual ITextToSpeech &TTS() = 0;

    /**
    * \brief Log the account out of the Vivox system. This will not raise a property event for changing the State to LoginState.LoggedOut, although
    * the state will be set to that upon completion of the function.
    */
    virtual void Logout() = 0;

    /**
    * \brief Get a login token for this account.
    *
    * \param tokenSigningKey the key corresponding to the issuer for this account that is used to sign the token
    * \param tokenExpirationDuration the length of time the token is valid for.
    * \return an access token that can be used to log this account in
    * \remarks To be used only by applications without secure communications requirements.
    */
    virtual FString GetLoginToken(const FString &tokenSigningKey, FTimespan tokenExpirationDuration) const = 0;
};
