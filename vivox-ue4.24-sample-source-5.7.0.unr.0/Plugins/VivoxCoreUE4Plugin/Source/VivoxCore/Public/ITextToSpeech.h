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
#include "ITTSMessage.h"

/**
 * \brief Interface for events and methods related to Text-To-Speech
 */
class VIVOXCORE_API ITextToSpeech
{
public:
    DECLARE_EVENT_ThreeParams(ITextToSpeech, PlaybackStarted, unsigned int, const ITTSMessage &, double);
    DECLARE_EVENT_TwoParams(ITextToSpeech, PlaybackCompleted, unsigned int, const ITTSMessage &);
    DECLARE_EVENT_TwoParams(ITextToSpeech, PlaybackFailed, VivoxCoreError, const ITTSMessage &);
public:
    /**
     * \brief destructor
     */
    virtual ~ITextToSpeech() = default;

    /**
     * \brief All voices available to the Text-To-Speech service for speech synthesis, indexed by name
     */
    virtual const TMap<FString, ITTSVoice*> &GetAvailableVoices() = 0;

    /**
     * \brief The voice used by Text-To-Speech methods called from this ILoginSession
     * \return The voice previously set with SetCurrentVoice(), or the SDK default voice if never set.
     */
    virtual const ITTSVoice &GetCurrentVoice() = 0;

    /**
     * \brief Sets the voice used by Text-To-Speech methods called from this ILoginSession
     * \param newVoice a valid ITTSVoice obtained from GetAvailableVoices() to use as this user's voice.
     * \return true if new value set, false if voice not available (possible, for instance, when loaded from saved settings).
     */
    virtual bool SetCurrentVoice(const ITTSVoice &newVoice) = 0;

    /**
     * \brief Injects a new Text-To-Speech message in the specified destination
     * \param text The text to be converted into speech.
     * \param destination determines the output stream and mechanism for TTS injection.
     * \param outMessage [out, optional] An object holding properties related to this TTS message.
     * \remarks See GetCurrentVoice() for how the ITTSVoice used for speech synthesis will be selected.
     */
    virtual VivoxCoreError Speak(const FString &text, const TTSDestination &destination, ITTSMessage **outMessage = nullptr) = 0;

    /**
     * \brief Injects a new Text-To-Speech message into an audio buffer for direct use
     * \param text The text to be converted into speech.
     * \param outBuffer [out] An object containing raw audio data and metadata such as buffer length and audio format properties.
     * \remarks See GetCurrentVoice() for how the ITTSVoice used for speech synthesis will be selected.
     */
    virtual VivoxCoreError SpeakToBuffer(const FString &text, ITTSAudioBuffer **outBuffer) = 0;

    /**
     * \brief Cancels a single currently playing or enqueued Text-To-Speech message
     * \param message The ITTSMessage to cancel.
     * \remarks In destinations with queues, cancelling an ongoing message will automatically trigger playback of
     * the next message. Cancelling an enqueued message will shift all later messages up one place in the queue.
     */
    virtual VivoxCoreError CancelMessage(const ITTSMessage &message) = 0;

    /**
     * \brief Cancels all Text-To-Speech messages in a destination (ongoing and enqueued)
     * \param destination The TTSDestination to clear of messages.
     * \remarks The TTSDestinations QueuedRemoteTransmission and QueuedRemoteTransmissionWithLocalPlayback
     * share a queue but are not the same destination. Canceling all messages in one of these destinations
     * will automatically trigger playback of the next message from the other in the shared queue.
     */
    virtual VivoxCoreError CancelDestination(const TTSDestination &destination) = 0;

    /**
     * \brief Cancels all Text-To-Speech messages (ongoing and enqueued) from all destinations
     */
    virtual VivoxCoreError CancelAll() = 0;

    /**
     * \brief Retrieves ongoing or enqueued TTS messages previously created via Speak()
     * \param destination The TTSDestination to retrieve messages for.
     * \return An array of all TTS messages playing or enqueued in the specified destination.
     */
    virtual const TArray<ITTSMessage *> &GetMessages(const TTSDestination &destination) = 0;

    /**
     * \brief An event indicating that a TTS message has finished preparing for playback and is just beginning to play
     * \param numConsumers The number of active sessions (for remote playback destinations) or local players
     * (for local playback destinations), or both, that the TTS message is playing into.
     * \param message The ITTSMessage corresponding to this speech.
     * \param messageDuration The duration of the synthesized voice clip in seconds.
     */
    PlaybackStarted EventPlaybackStarted;

    /**
     * \brief An event indicating that a TTS message has finished playback
     * \param numConsumers The number of active sessions (for remote playback destinations) or local players
     * (for local playback destinations), or both, that the TTS message is playing into.
     * \param message The ITTSMessage corresponding to this speech.
     * \remarks This event is raised no matter if playback is stopped early via the various Cancel*() methods, if it is
     * being replaced by a new message (TTSDestination.ScreenReader only), or if the voice clip has reached its natural end.
     * The correponding ITTSMessage will no longer be obtainable via GetMessages() once this event is received.
     */
    PlaybackCompleted EventPlaybackCompleted;

    /**
     * \brief An event indicating that playback of a TTS message has failed
     * \param status The error code of the failure.
     * \param message The ITTSMessage corresponding to this speech.
     * \remarks The correponding ITTSMessage will no longer be obtainable via GetMessages() once this event is received.
     */
    PlaybackFailed EventPlaybackFailed;
};
