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
#include "ITTSVoice.h"

typedef vx_tts_voice_t TTSVoiceStruct;

/**
  * \brief A Voice used by the Text-To-Speech service to synthesize speech
  */
class TTSVoiceImpl : public ITTSVoice
{
    FString _name;
    TTSVoiceId _voiceId;
    bool _isValid;

public:
    TTSVoiceImpl();
    TTSVoiceImpl(const TTSVoiceStruct &voiceStruct);
    TTSVoiceImpl(const ITTSVoice &ttsVoice);
    ~TTSVoiceImpl();

    const FString &Name() const override;
    const TTSVoiceId &VoiceId() const override;
    bool IsValid() const override;

    TTSVoiceImpl &operator =(const ITTSVoice &ttsVoice);
};
