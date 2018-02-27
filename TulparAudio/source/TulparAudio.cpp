/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/TulparAudio.hpp>

#include <tulpar/internal/Collection.hpp>

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

void TulparAudio::Initialize(uint32_t bufferBatch, uint32_t sourceBatch)
{
    assert(false == m_isInitialized);

    m_buffers.reset(new internal::BufferCollection());
    m_buffers->Initialize(bufferBatch);

    m_sources.reset(new internal::SourceCollection(*m_buffers));
    m_sources->Initialize(sourceBatch);
}

void TulparAudio::Deinitialize()
{
    if (m_isInitialized)
    {
        m_sources.reset();
        m_buffers.reset();

        m_isInitialized = true;
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
