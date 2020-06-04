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

/**
  * \brief The distance model for a Positional channel, which determines the algorithm to use when computing attenuation
  */
UENUM(BlueprintType)
enum class EAudioFadeModel : uint8
{
    /**
     * \brief Fades voice quickly at first, buts slows down as you get further from conversational distance.
     */
    InverseByDistance = 0,
    /**
     * \brief Fades voice slowly at first, but speeds up as you get further from conversational distance.
     */
    LinearByDistance,
    /**
     * \brief Makes voice within the conversational distance louder, but fade quickly beyond it.
     */
    ExponentialByDistance
};

/**
  * \brief Properties to control the 3D effects applied to audio in Positional Channels
  */
class VIVOXCORE_API Channel3DProperties
{
    int32 _audibleDistance;
    int32 _conversationalDistance;
    double _audioFadeIntensityByDistance;
    EAudioFadeModel _audioFadeModel;
public:
    /**
     * \brief Default Constructor
     */
    Channel3DProperties();
    /**
     * \brief Constructor used to set all 3D channel properties. Please refer to the Vivox Core Unreal Developer Guide for recommendations on picking values for different 3D scenarios.
     * \param audibleDistance The maximum distance from the listener that a speaker can be heard. Must be > 0.
     * \param conversationalDistance The distance from the listener within which a speaker’s voice is heard at its original volume. Must be >= 0 and <= audibleDistance.
     * \param audioFadeIntensityByDistance How strong the effect of the audio fade is as the speaker moves away from the listener. Must be >= 0. This value will be rounded to three decimal places.
     * \param audioFadeModel The model used to determine how loud a voice is at different distances.
	 * \remarks See remarks of individual property access methods, e.g. AudioFadeModel(), for more detail on each property.
     */
    Channel3DProperties(int32 audibleDistance, int32 conversationalDistance, double audioFadeIntensityByDistance, EAudioFadeModel audioFadeModel);
    /**
     * \brief Destructor
     */
    ~Channel3DProperties() {}
    /**
     * \brief The maximum distance from the listener that a speaker can be heard.
     * \remarks Any players within this distance from you in any direction will appear in the same positional
     *  voice channel as you and may be heard. When a player crosses this threshold distance from your perspective,
     *  an IChannelSession event will fire, either EventParticipantAdded when a player comes within this distance,
     *  or EventParticipantLeft when a player moves beyond it. You will stop receiving audio from participants
     *  beyond this range, even before the participant left event is called, but are guaranteed to receive the
     *  added event before receiving audio. The value of this property is measured in arbitrary “distance units,”
     *  so can be set to any scale and does not need to conform to any real units. The default value is 2700.
     */
    int32 AudibleDistance() const { return _audibleDistance; }
    /**
     * \brief The distance from the listener within which a speaker’s voice is heard at its original volume, and beyond which the speaker's voice begins to fade.
     * \remarks This property is measured in arbitrary “distance units,” but should use the same scale as
     *  audibleDistance. Your 3D audio experience will sound the most realistic when the value of this property
     *  is set to half the height of a typical player avatar in your game. For near-human-sized entities, this
     *  means about 1 meter, 90 centimeters, or 3 feet. The default value is 90.
     */
    int32 ConversationalDistance() const { return _conversationalDistance; }
    /**
     * \brief How strong the effect of the audio fade is as the speaker moves away from the listener, past the conversational distance. 1=normal strength, .5=half strength, 2=double strength, etc.
     * \remarks This parameter is a scalar used in the audio fade calculations as either a constant multiplier, or
     *  an exponent, depending on the audioFadeModel used. In this way, it scales up or down the result of the audio
     *  attenuation at different distances, as determined by the model's formula. Values greater than 1.0 mean audio
     *  will fade quicker as you move away from the conversational distance, and less than 1.0 mean that is will
     *  fade slower. The default value is 1.0.
     */
    double AudioFadeIntensityByDistance() const { return _audioFadeIntensityByDistance; }
    /**
     * \brief The model used to determine how loud a voice is at different distances.
     * \remarks Voice heard within the conversationalDistance is at the original speaking volume, and voice from speakers
     *  past the audibleDistance will no longer be transmitted. The loudness of the audio at every other distance within
     *  this range is controlled by one of three possible audio fade models. The default value is InverseByDistance, which
     *  is the most realistic.
     *  - InverseByDistance
     *          - Fades voice quickly at first, buts slows down as you get further from conversational distance. Formally,
     *            the attenuation increases in inverse proportion to the distance. This option models real life acoustics,
     *            and will sound the most natural.
     *  - LinearByDistance
     *          - Fades voice slowly at first, but speeds up as you get further from conversational distance. Formally,
     *            the attenuation increases in linear proportion to the distance.The audioFadeIntensityByDistance factor is
     *            the negative slope of the attenuation curve. This option can be thought of as a compromise between
     *            realistic acoustics and a radio channel with no distance attenuation.
     *  - ExponentialByDistance
     *          - Fades voice extremely quickly beyond conversational distance + 1. Formally, the attenuation increases in
     *            inverse proportion to the distance raised to the power of the audioFadeIntensityByDistance factor. It
     *            shares a curve shape similar to realistic attenuation, but allows for much steeper rolloff. This option
     *            can be used to apply a 'cocktail party effect' to the audio space; by tuning the
     *            audioFadeIntensityByDistance, this model allows nearby participants to be understandable while mixing
     *            farther participants’ conversation into a bearable and non-intrusive chatter.
     */
    EAudioFadeModel AudioFadeModel() const { return _audioFadeModel; }
    /**
     * \brief Internal Use Only
     * \remarks This function creates a Channel3DProperties object from the portion of the URI used to maintain that information
     */
    static Channel3DProperties CreateFromString(FString propsString);
    /**
     * \brief Internal Use Only
     */
    bool IsValid() const;
    /**
     * \brief Internal Use Only
     */
    FString ToString() const;
};
