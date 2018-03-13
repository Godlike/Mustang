/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/Context.hpp>

#include <cassert>
#include <cstddef>

namespace tulpar
{
namespace internal
{

Context::Context()
    : m_isInitialized(false)
    , m_pContext(nullptr)
{

}

Context::~Context()
{
    Deinitialize();
}

bool Context::Initialize(Device& device)
{
    Deinitialize();

    ALCdevice* pDevice = device.GetOpenALDevice();

    // flush error flag
    ALCenum alcErr = alcGetError(pDevice);

    m_pContext = alcCreateContext(pDevice, NULL);

    alcErr = alcGetError(pDevice);

    if (ALC_NO_ERROR == alcErr)
    {
        m_isInitialized = true;
    }

    return m_isInitialized;
}

void Context::Deinitialize()
{
    if (m_isInitialized)
    {
        alcMakeContextCurrent(NULL);

        if (m_pContext)
        {
            alcDestroyContext(m_pContext);
            m_pContext = nullptr;
        }

        m_isInitialized = false;
    }
}

void Context::MakeCurrent()
{
    assert(true == m_isInitialized);

    alcMakeContextCurrent(m_pContext);
}

}
}
