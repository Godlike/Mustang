/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/Device.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <cstddef>

namespace tulpar
{
namespace internal
{

Device::Device()
    : m_isInitialized(false)
    , m_pDevice(nullptr)
{

}

Device::~Device()
{
    Deinitialize();
}

bool Device::Initialize()
{
    assert(false == m_isInitialized);

    LOG_AUDIO->Debug("Device::Initialize() started");

    // clear error state
    ALCenum alcErr = alcGetError(NULL);

    m_pDevice = alcOpenDevice(NULL);

    alcErr = alcGetError(m_pDevice);

    if (ALC_NO_ERROR == alcErr)
    {
        m_isInitialized = true;

        LOG_AUDIO->Debug("Device::Initialize() done {:#x}", reinterpret_cast<uintptr_t>(m_pDevice));
    }
    else
    {
        LOG_AUDIO->Error("Device::Initialize() failed: {:#x}", alcErr);
    }

    return m_isInitialized;
}

void Device::Deinitialize()
{
    if (m_isInitialized)
    {
        LOG_AUDIO->Debug("Device::Deinitialize() started {:#x}", reinterpret_cast<uintptr_t>(m_pDevice));

        // clear error state
        ALCenum alcErr = alcGetError(m_pDevice);

        alcCloseDevice(m_pDevice);

        alcErr = alcGetError(m_pDevice);

        if (ALC_NO_ERROR != alcErr)
        {
            LOG_AUDIO->Error("Device::Deinitialize() {:#x} failed: {:#x}", reinterpret_cast<uintptr_t>(m_pDevice), alcErr);
        }

        m_pDevice = nullptr;

        m_isInitialized = false;
    }
}

}
}
