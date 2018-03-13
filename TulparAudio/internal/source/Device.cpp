/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/Device.hpp>

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
    Deinitialize();

    // flush error flag
    ALCenum alcErr = alcGetError(NULL);

    m_pDevice = alcOpenDevice(NULL);

    alcErr = alcGetError(m_pDevice);

    if (ALC_NO_ERROR == alcErr)
    {
        m_isInitialized = true;
    }

    return m_isInitialized;
}

void Device::Deinitialize()
{
    if (m_isInitialized)
    {
        if (m_pDevice)
        {
            alcCloseDevice(m_pDevice);
            m_pDevice = nullptr;
        }

        m_isInitialized = false;
    }
}

}
}
