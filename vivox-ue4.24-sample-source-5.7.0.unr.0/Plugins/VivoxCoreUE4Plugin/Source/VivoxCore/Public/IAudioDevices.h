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
#include "IAudioDevice.h"
#include "VivoxCoreCommon.h"

/**
 * \brief This interface is used to enumerate and manage audio devices
 */
class VIVOXCORE_API IAudioDevices
{
public:
    /* @cond */
    DECLARE_EVENT_OneParam(IAudioDevices, AfterAvailableDeviceAdded, const IAudioDevice &)
    DECLARE_EVENT_OneParam(IAudioDevices, BeforeAvailableDeviceRemoved, const IAudioDevice &)
    DECLARE_EVENT_OneParam(IAudioDevices, EffectiveDeviceChanged, const IAudioDevice &)
    DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateSetActiveDeviceCompleted, VivoxCoreError, const FString &)
    /* @endcond */

public:
    virtual ~IAudioDevices() = default;

    /**
     * \brief Call SetActiveDevice() with this device if you wish to use the operating system's default device.
     * \remarks Name and Id show the current values of the underlying device this virtual device is tracking.
     */
    virtual const IAudioDevice &SystemDevice() const = 0;

    /**
    * \brief Call SetActiveDevice() with this device if you wish to use the operating system's default communication device
    * \remarks Name and Id show the current values of the underlying device this virtual device is tracking.
    * This always points to the same physical device as SystemDevice except on Windows, UWP, and Xbox One, where it may differ.
    */
    virtual const IAudioDevice &CommunicationDevice() const = 0;

    /**
     * \brief The available devices on this system. Lists both "virtual" and physical devices.
     * \remarks Key equals the IAudioDevice Value's Id(). Virtual devices appear on the available device list
     * with their virtual Name and Id, e.g. "Default System Device" whereas the IAudioDevice reference
     * returned by SystemDevice() and similar methods show what the effective device would be upon setting
     * it active. You may use either IAudioDevice with SetActiveDevice() to set a virtual device active.
     */
    virtual const TMap<FString, IAudioDevice *> &AvailableDevices() const = 0;

    /**
     * \brief This event is raised after a device has been added to the AvailableDevices() collection.
     */
    AfterAvailableDeviceAdded EventAfterDeviceAvailableAdded;

    /**
     * \brief This event is raised before a device will be removed from the AvailableDevices() collection.
     */
    BeforeAvailableDeviceRemoved EventBeforeAvailableDeviceRemoved;

    /**
     * \brief The delegate called when SetActiveDevice Completes. FString equals new ActiveDevice().Id().
     */
    typedef FDelegateSetActiveDeviceCompleted::FDelegate FOnSetActiveDeviceCompletedDelegate;

    /**
     * \brief Call this to set the active audio device. This will take effect immediately for all open sessions.
     * \param device The device to set active
     * \param theDelegate a delegate to call when this operation completes.
     * \return VxErrorInvalidArgument if device is empty
     */
    virtual VivoxCoreError SetActiveDevice(const IAudioDevice &device, const FOnSetActiveDeviceCompletedDelegate& theDelegate = FOnSetActiveDeviceCompletedDelegate()) = 0;

    /**
     * \brief The active audio device
     */
    virtual const IAudioDevice &ActiveDevice() = 0;

    /**
     * \brief The effective audio device. If the active device is set to SystemDevice or CommunicationDevice, then the effective device will reflect the actual device used.
     */
    virtual const IAudioDevice &EffectiveDevice() = 0;

    /**
    * \brief When the effective device changed, this event is fired.
    * Use this to inform users that the device in use has changed.
    */
    EffectiveDeviceChanged EventEffectiveDeviceChanged;

    /**
     * \brief AudioGain for the device.
     * This is a value between -50 and 50. Positive values make the audio louder, negative values make the audio softer.
     * 0 leaves the value unchanged (default).
     * This applies to all active audio sessions.
     */
    virtual int VolumeAdjustment() = 0;

    /**
    * \brief Set AudioGain for the device.
    * This is a value between -50 and 50. Positive values make the audio louder, negative values make the audio softer.
    * 0 leaves the value unchanged (default).
    * This applies to all active audio sessions.
    * \param value the desired audio gain
    * \return VxErrorInvalidArgument if value is outside of -50 -> 50
    */
    virtual VivoxCoreError SetVolumeAdjustment(int value) = 0;

    /**
     * \brief Get whether audio is muted for this device.
     */
    virtual bool Muted() const = 0;
    /**
     * \brief Set whether audio is muted for this device.
     *
     * Set to true in order to stop the audio device from capturing or rendering audio.
     * Default is false.
     */
    virtual void SetMuted(bool value) = 0;
    /**
    * \brief Refresh the list of available devices.
    *
    * \remarks Manual use of this method should be rare, as the device lists are refreshed automatically
    * when Vivox is initialized and when an auto device hotswap event is raised by the system.
    * Do note however that an automatic or manual call must complete before IAudioDevices methods for
    * ActiveDevice, EffectiveDevice, AvailableDevices etc, will return valid values. It may take up to 
    * 200 milliseconds before the list of devices is refreshed.
    */
    virtual void Refresh() = 0;
};
