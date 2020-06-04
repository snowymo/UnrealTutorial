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
#include "Vxc.h"

 // Added with the adoption of Core 5.3.0+ to preserve backwards compatibility
 // until we can properly deprecate this codec enum for a wrapped vx_codec.
enum media_codec_type {
    media_codec_type_none = 0,
    media_codec_type_siren14 = 1,
    media_codec_type_pcmu = 2,
    media_codec_type_nm = 3,
    media_codec_type_speex = 4,
    media_codec_type_siren7 = 5,
    media_codec_type_opus = 6
};

class VIVOXCORE_API VivoxConfig
{
    vx_sdk_config_t config;
    static void LoggingCallback(void *callback_handle, vx_log_level logLevel, const char *source, const char *message);
    static void* VivoxMalloc(size_t bytes);
    static void VivoxFree(void* ptr);
    static void* VivoxRealloc(void* ptr, size_t bytes);
    static void* VivoxCalloc(size_t num, size_t bytes);
    static void* VivoxMallocAligned(size_t alignment, size_t bytes);
    static void VivoxFreeAligned(void* ptr);
public:
    VivoxConfig();
    ~VivoxConfig() {}

    /**
    * \brief Retrieves the underlying Vivox config.
    */
    vx_sdk_config_t ToVx_Sdk_Config() { return config; }
    /**
    * \brief The log level of the SDK.
    * Severity level of logs: -1 = no logging, 0 = errors only, 1 = warnings, 2 = info, 3 = debug, 4 = trace, 5 = log all
    */
    vx_log_level LogLevel() const { return config.initial_log_level; }
    /**
    * \brief The default codec mask.
    * codec type none = 0, codec type siren14 = 1, codec type pcmu = 2, codec type nm = 3,
    * codec type speex = 4, codec type siren7 = 5, codec type opus = 6
    * CAUTION: Altering this value may result in unexpected behavior. 
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    media_codec_type DefaultCodecsMask() const {
        switch (vx_codec(config.default_codecs_mask))
        {
        case vx_codec_pcmu:
            return media_codec_type_pcmu;
        case vx_codec_siren7:
            return media_codec_type_siren7;
        case vx_codec_siren14:
            return media_codec_type_siren14;
        case vx_codec_opus8:
        case vx_codec_opus40:
        case vx_codec_opus57:
        case vx_codec_opus72:
            return media_codec_type_opus;
        default:
            return media_codec_type(config.default_codecs_mask);
        }
    }

    /**
    * \brief The render source max queue depth.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int RenderSourceQueueDepthMax() const { return config.render_source_queue_depth_max; }
    /**
    * \brief The render source initial buffer count.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int RenderSourceInitialBufferCount() const { return config.render_source_initial_buffer_count; }
    /**
    * \brief The upstream jitter frame count.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int UpstreamJitterFrameCount() const { return config.upstream_jitter_frame_count; }
    /**
    * \brief The max number of logins allowed for a user.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int MaxLoginsPerUser() const { return config.max_logins_per_user; }
    /**
    * \brief The number of 20 millisecond buffers for the capture device.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int CaptureDeviceBufferSizeIntervals() const { return config.capture_device_buffer_size_intervals; }
    /**
    * \brief The number of 20 millisecond buffers for the render device.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    int RenderDeviceBufferSizeIntervals() const { return config.render_device_buffer_size_intervals; }
    /**
    * \brief The number of milliseconds to wait before disconnecting audio due to RTP timeout at initial call time. Zero or negative value turns off the guard (not recommended).
    */
    int NeverRtpTimeoutMs() const { return config.never_rtp_timeout_ms; }
    /**
    * \brief The number of milliseconds to wait before disconnecting audio due to RTP timeout after the call has been established. Zero or negative value turns off the guard (not recommended).
    */
    int LostRtpTimeoutMs() const { return config.lost_rtp_timeout_ms; }
    /**
    * \brief Processor Affinity Mask for SDK Threads
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    long long ProcessorAffinityMask() const { return config.processor_affinity_mask; }
    /**
    * \brief Status of device polling.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool DisableDevicePolling() const { return config.disable_device_polling; }
    /**
    * \brief Status of capturing silence.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool ForceCaptureSilence() const { return config.force_capture_silence; }
    /**
    * \brief Status of advanced automatic settings of audio levels
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool EnableAdvancedAutoLevels() const { return config.enable_advanced_auto_levels; }
    /**
    * \brief Enable Persistent Connections (Windows Only) (default: 0 or 1 if environment variable "VIVOX_ENABLE_PERSISTENT_HTTP" is set
    * Note that the use of proxies may interfere with behavior controlled by this setting.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool EnablePersistentHTTP() const { return config.enable_persistent_http; }
    /**
    * \brief Status of audio ducking for XBox One and iOS.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool DisableAudioDucking() const { return config.disable_audio_ducking; }
    /**
    * \brief Status of fast network change detection. Default of disabled.
    */
    bool EnableFastNetworkChangeDetection() const { return config.enable_fast_network_change_detection; }
    /**
    * \brief Use Operating System Configured Proxy Settings (Windows Only) (default: 0 or 1 if environment variable "VIVOX_USE_OS_PROXY_SETTINGS" is set)
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool UseOSProxySettings() const { return config.use_os_proxy_settings; }
    /**
    * \brief Set to true to use the pooled allocator (default false)
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */  
    bool UsePooledAllocator() const { return config.use_pooled_allocator; }
    /**
    * \brief Allow shared capture devices (shared in the Vivox context only).
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool AllowSharedCaptureDevices() const { return config.allow_shared_capture_devices; }
    /**
    * \brief Default of true for most platforms.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    bool EnableDTX() const { return config.enable_dtx; }


    /**
    * \brief Sets the log level of the Vivox SDK.
    * Severity level of logs: -1 = no logging, 0 = errors only, 1 = warnings, 2 = info, 3 = debug, 4 = trace, 5 = log all
    */
    void SetLogLevel(vx_log_level logLevel);

    /**
    * \brief Set the codec mask that will be used to initialize connector's configured_codecs.
    * codec type none = 0, codec type siren14 = 1, codec type pcmu = 2, codec type nm = 3,
    * codec type speex = 4, codec type siren7 = 5, codec type opus = 6
    * CAUTION: Altering this value will change the quality of the voice experience.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetDefaultCodecsMask(media_codec_type codecType);

    /**
    * \brief Set the Render Source Max Queue Depth.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetRenderSourceQueueDepthMax(int queueDepthMax);

    /**
    * \brief Set the Render Source Initial Buffer Count.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetRenderSourceInitialBufferCount(int initialBufferCount);

    /**
    * \brief Set the Upstream jitter frame count
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetUpstreamJitterFrameCount(int upstreamJitterFrameCount);

    /**
    * \brief Set the max logins a user can have
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetMaxLoginsPerUser(int maxLoginsPerUser);

    /**
    * \brief Set the number of 20 millisecond buffers for the capture device.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetCaptureDeviceBufferSizeIntervals(int captureDeviceBufferSizeIntervals);

    /**
    * \brief Set the number of 20 millisecond buffers for the render device.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetRenderDeviceBufferSizeIntervals(int renderDeviceBufferSizeIntervals);

    /**
    * \brief Set the number of milliseconds to wait before disconnecting audio due to RTP timeout at initial call time. Zero or negative value turns off the guard (not recommended).
    */
    void SetNeverRtpTimeoutMs(int neverRtpTimeoutMs);

    /**
    * \brief Set the number of milliseconds to wait before disconnecting audio due to RTP timeout after the call has been established. Zero or negative value turns off the guard (not recommended).
    */
    void SetLostRtpTimeoutMs(int lostRtpTimeoutMs);

    /**
    * \brief Processor Affinity Mask for SDK Threads
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetProcessorAffinityMask(long long processorAffinityMask);

    /**
    * \brief Set whether or not to Disable Audio Device Polling Using Timer
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetDisableDevicePolling(bool disableDevicePolling);

    /**
    * \brief Set whether to force capturing silence. Diagnostic purposes only.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetForceCaptureSilence(bool forceCaptureSilence);

    /**
    * \brief Set whether to enable advanced automatic settings of audio levels.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetEnableAdvancedAutoLevels(bool enableAdvancedAutoLevels);

    /**
    * \brief Enable Persistent Connections (Windows Only) (default: false. True if environment variable "VIVOX_ENABLE_PERSISTENT_HTTP" is set
    * Note that the use of proxies may interfere with behavior controlled by this setting.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetEnablePersistentHTTP(bool enablePersistentHTTP);

    /**
    * \brief Set whether to disable Audio Ducking. XBox One and iOS.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetDisableAudioDucking(bool disableAudioDucking);

    /**
    * \brief Set whether to enable Fast Network Change Detection. Default of disable.
    */
    void SetEnableFastNetworkChangeDetection(bool enableFastNetworkChangeDetection);

    /**
    * \brief Use Operating System Configured Proxy Settings (Windows Only) (default: 0 or 1 if environment variable "VIVOX_USE_OS_PROXY_SETTINGS" is set)
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetUseOSProxySettings(bool useOSProxySettings);

    /**
    * \brief Set to true to use the pooled allocator (default false)
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetUsePooledAllocator(bool usePooledAllocator);

    /**
    * \brief Allow shared capture devices (shared in the Vivox context only).
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetAllowSharedCaptureDevices(bool allowSharedCaptureDevices);

    /**
    * \brief Default of true for most platforms.
    * CAUTION: Altering this value may result in unexpected behavior.
    * For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
    */
    void SetEnableDTX(bool enableDTX);
};
