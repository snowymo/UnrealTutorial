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

/**
  * \brief Either an audio input device (microphone) or audio output device (speaker or headphones)
  */
class VIVOXCORE_API IAudioDevice
{
public:
    virtual ~IAudioDevice() = default;
    /**
     * \brief User displayable device name
     */
    virtual const FString &Name() const = 0;
    /**
     * \brief Internal Device Identifier
     */
    virtual const FString &Id() const = 0;
    /**
     * \brief Returns true iff both the Name and Id properties are empty
     */
    virtual bool IsEmpty() const = 0;
};
