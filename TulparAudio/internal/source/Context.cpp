/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/Context.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <cassert>
#include <cstdint>
#include <cstddef>

namespace tulpar
{
namespace internal
{

Context* Context::Create(Device const& device)
{
    Context* obj = new Context();

    if (obj->Initialize(device))
    {
        return obj;
    }
    else
    {
        delete obj;

        return nullptr;
    }
}

Context::~Context()
{
    Deinitialize();
}

void Context::MakeCurrent()
{
    assert(true == m_isInitialized);

    LOG_AUDIO->Debug("Context::MakeCurrent() {:#x}", reinterpret_cast<uintptr_t>(m_pContext));

    // clear error state
    ALCenum alcErr = alcGetError(m_pDevice);

    alcMakeContextCurrent(m_pContext);

    alcErr = alcGetError(m_pDevice);

    if (ALC_NO_ERROR != alcErr)
    {
        LOG_AUDIO->Debug("Context::MakeCurrent() {:#x} failed: {:#x}", reinterpret_cast<uintptr_t>(m_pContext), alcErr);
    }
}

Context::Context()
    : m_isInitialized(false)
    , m_pContext(nullptr)
    , m_pDevice(nullptr)
{

}

bool Context::Initialize(Device const& device)
{
    Deinitialize();

    m_pDevice = device.GetOpenALDevice();

    LOG_AUDIO->Debug("Context::Initialize(device = {:#x}) started", reinterpret_cast<uintptr_t>(m_pDevice));

    // clear error state
    ALCenum alcErr = alcGetError(m_pDevice);

    m_pContext = alcCreateContext(m_pDevice, NULL);

    alcErr = alcGetError(m_pDevice);

    if (ALC_NO_ERROR == alcErr)
    {
        m_isInitialized = true;

        LOG_AUDIO->Debug("Context::Initialize(device = {:#x}) done {:#x}", reinterpret_cast<uintptr_t>(m_pDevice), reinterpret_cast<uintptr_t>(m_pContext));
    }
    else
    {
        LOG_AUDIO->Error("Context::Initialize(device = {:#x}) failed: {:#x}", reinterpret_cast<uintptr_t>(m_pDevice), alcErr);
    }

    return m_isInitialized;
}

void Context::Deinitialize()
{
    if (m_isInitialized)
    {
        LOG_AUDIO->Debug("Context::Deinitialize() started {:#x}", reinterpret_cast<uintptr_t>(m_pContext));

        alcMakeContextCurrent(NULL);

        // clear error state
        ALCenum alcErr = alcGetError(m_pDevice);

        alcDestroyContext(m_pContext);

        alcErr = alcGetError(m_pDevice);

        if (ALC_NO_ERROR != alcErr)
        {
            LOG_AUDIO->Error("Context::Deinitialize() {:#x} failed: {:#x}", reinterpret_cast<uintptr_t>(m_pContext), alcErr);
        }

        LOG_AUDIO->Debug("Context::Deinitialize() done {:#x}", reinterpret_cast<uintptr_t>(m_pContext));

        m_pContext = nullptr;

        m_isInitialized = false;
    }
}

}
}
