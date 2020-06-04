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
#include "ITTSMessage.h"
#include "TTSVoiceImpl.h"

/**
 * \brief A Voice used by the Text-To-Speech service to synthesize speech
 */
class TTSMessageImpl : public ITTSMessage
{
    TTSVoiceImpl _voice;
    FString _text;
    TTSDestination _destination;
    TTSUtteranceId _utteranceId;
    TTSMessageState _state;
    bool _isValid;

public:
    TTSMessageImpl();
    TTSMessageImpl(const TTSVoiceImpl &voice, const FString &text, const TTSDestination &destination, const TTSUtteranceId &utteranceId, const TTSMessageState &state);
    ~TTSMessageImpl();

    const ITTSVoice &Voice() const override;
    const FString &Text() const override;
    const TTSDestination &Destination() const override;
    const TTSUtteranceId &UtteranceId() const override;
    const TTSMessageState &State() const override;
    bool IsValid() const override;

    // internal
    void SetPlaying();
};
