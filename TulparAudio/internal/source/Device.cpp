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

Device* Device::Create(TulparConfigurator::Device const& config)
{
    Device* obj = new Device();

    if (obj->Initialize(config.name))
    {
        return obj;
    }
    else
    {
        delete obj;

        return nullptr;
    }
}

Device::~Device()
{
    Deinitialize();
}

Device::Device()
    : m_isInitialized(false)
    , m_pDevice(nullptr)
{

}

bool Device::Initialize(std::string const& name)
{
    assert(false == m_isInitialized);

    LOG_AUDIO->Debug("Device::Initialize({}) started", name.c_str());

    // clear error state
    ALCenum alcErr = alcGetError(NULL);

    m_pDevice = alcOpenDevice(name.empty() ? NULL : name.c_str());

    alcErr = alcGetError(m_pDevice);

    if (ALC_NO_ERROR == alcErr)
    {
        m_isInitialized = true;

        LOG_AUDIO->Debug("Device::Initialize({}) done {:#x}", name.c_str(), reinterpret_cast<uintptr_t>(m_pDevice));
    }
    else
    {
        LOG_AUDIO->Error("Device::Initialize({}) failed: {:#x}", name.c_str(), alcErr);
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
