/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/TulparAudio.hpp>

#include <tulpar/internal/BufferCollection.hpp>
#include <tulpar/internal/Context.hpp>
#include <tulpar/internal/Device.hpp>
#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/InternalLoggers.hpp>
#include <tulpar/Loggers.hpp>

#include <cassert>

namespace tulpar
{

TulparAudio::TulparAudio()
    : m_isInitialized(false)
    , m_buffers(nullptr)
    , m_sources(nullptr)
{

}

TulparAudio::~TulparAudio()
{
    Deinitialize();
}

bool TulparAudio::Initialize(uint32_t bufferBatch, uint32_t sourceBatch)
{
    assert(false == m_isInitialized);

    Loggers::Instance().Reinitialize();

    LOG->Trace("TulparAudio::Initialize(bufferBatch = {}, sourceBatch = {}) started", bufferBatch, sourceBatch);

    m_pDevice = new internal::Device();
    m_isInitialized = m_pDevice->Initialize();

    if (m_isInitialized)
    {
        m_pContext = new internal::Context();
        m_isInitialized = m_pContext->Initialize(*m_pDevice);

        if (m_isInitialized)
        {
            m_pContext->MakeCurrent();

            m_buffers.reset(new internal::BufferCollection());
            m_buffers->Initialize(bufferBatch);

            m_sources.reset(new internal::SourceCollection(*m_buffers));
            m_sources->Initialize(sourceBatch);

            m_isInitialized = true;
        }
    }

    if (false == m_isInitialized)
    {
        LOG->Error("TulparAudio::Initialize(bufferBatch = {}, sourceBatch = {}) failed", bufferBatch, sourceBatch);

        m_isInitialized = true;
        Deinitialize();
    }
    else
    {
        LOG->Debug("TulparAudio::Initialize(bufferBatch = {}, sourceBatch = {}) done", bufferBatch, sourceBatch);
    }

    return m_isInitialized;
}

void TulparAudio::Deinitialize()
{
    if (m_isInitialized)
    {
        LOG->Trace("TulparAudio::Deinitialize() started");

        m_sources.reset();
        m_buffers.reset();

        delete m_pContext;
        m_pContext = nullptr;

        delete m_pDevice;
        m_pDevice = nullptr;

        m_isInitialized = false;

        LOG->Debug("TulparAudio::Deinitialize() done");
    }
}

audio::Source TulparAudio::GetSource(audio::Source::Handle handle) const
{
    assert(true == m_isInitialized);

    return m_sources->Get(handle);
}

audio::Source TulparAudio::SpawnSource()
{
    assert(true == m_isInitialized);

    return m_sources->Spawn();
}

audio::Buffer TulparAudio::GetBuffer(audio::Buffer::Handle handle) const
{
    assert(true == m_isInitialized);

    return m_buffers->Get(handle);
}

audio::Buffer TulparAudio::SpawnBuffer()
{
    assert(true == m_isInitialized);

    return m_buffers->Spawn();
}

}
