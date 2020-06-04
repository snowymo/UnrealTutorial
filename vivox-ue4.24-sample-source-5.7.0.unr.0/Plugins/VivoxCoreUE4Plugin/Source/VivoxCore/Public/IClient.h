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

/**
 * \brief Provides access to the Vivox Voice System.
 */
class VIVOXCORE_API IClient
{
public:
    virtual ~IClient() = default;
    /**
     * \brief Initializes this Client instance.
     * If the client is already initialized, it will do nothing but return VxErrorAlreadyInitialized.
     * \param sdkConfig the config to initialize with which allows for the customization of certain aspects of the Vivox SDK.
     * \return VxErrorSuccess under most circumstances. Failures to initialize are rare.
     */
    virtual VivoxCoreError Initialize(VivoxConfig sdkConfig = VivoxConfig()) = 0;
    /**
     * \brief Uninitialize this Client instance. If this Client instance is not initialized, it will do nothing.
     */
    virtual void Uninitialize() = 0;

    /**
     * \brief The internal version of the low level vivoxsdk library
     */
    static FString InternalVersion();

    /**
     * \brief The internal version of the low level vivoxsdk library with extended info
     */
    static FString InternalVersionExtended();

    /**
     * \brief Gets the LoginSession object for the provided accountId, creating one if necessary
     * \param loginSessionId the AccountId for this login session
     * \return the login session for that accountId
     * \remarks If a new LoginSession is created, LoginSessions.AfterKeyAdded will be raised.</remarks>
     */
    virtual ILoginSession &GetLoginSession(const AccountId &loginSessionId) = 0;

    /**
     * \brief All the Login Sessions associated with this Client instance.
     */
    virtual const TMap<AccountId, TSharedPtr<ILoginSession>> & LoginSessions() = 0;

    /**
     * \brief The Audio Input Devices associated with this Client instance
     */
    virtual IAudioDevices &AudioInputDevices() = 0;

    /**
     * \brief The Audio Output Devices associated with this Client instance
     */
    virtual IAudioDevices &AudioOutputDevices() = 0;
};
