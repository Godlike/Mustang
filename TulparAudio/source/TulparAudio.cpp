/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/TulparAudio.hpp>

#include <tulpar/internal/BufferCollection.hpp>
#include <tulpar/internal/Context.hpp>
#include <tulpar/internal/Device.hpp>
#include <tulpar/internal/ListenerController.hpp>
#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/InternalLoggers.hpp>
#include <tulpar/Loggers.hpp>

#include <cassert>

namespace tulpar
{

TulparAudio::TulparAudio()
    : m_isInitialized(false)
    , m_device(nullptr)
    , m_context(nullptr)
    , m_listener(new internal::ListenerController())
    , m_buffers(nullptr)
    , m_sources(nullptr)
{

}

TulparAudio::~TulparAudio()
{
    Deinitialize();
}

bool TulparAudio::Initialize(TulparConfigurator const& config)
{
    assert(false == m_isInitialized);

    LOG->Trace("TulparAudio::Initialize({}) started", config);

    m_device.reset(internal::Device::Create(config.device));

    if (nullptr != m_device.get())
    {
        m_context.reset(internal::Context::Create(*m_device));

        if (nullptr != m_context.get())
        {
            m_context->MakeCurrent();

            m_buffers.reset(new internal::BufferCollection());
            m_buffers->Initialize(config.bufferBatch);

            m_sources.reset(new internal::SourceCollection(*m_buffers));
            m_sources->Initialize(config.sourceBatch);

            m_isInitialized = true;
        }
    }

    if (false == m_isInitialized)
    {
        LOG->Error("TulparAudio::Initialize() failed");

        m_isInitialized = true;
        Deinitialize();
    }
    else
    {
        LOG->Debug("TulparAudio::Initialize() done");
    }

    return m_isInitialized;
}

bool TulparAudio::Reinitialize(TulparConfigurator const& config)
{
    assert(true == m_isInitialized);

    m_isInitialized = false;

    LOG->Trace("TulparAudio::Reinitialize({}) started", config);

    internal::Device* pDevice = internal::Device::Create(config.device);

    if (nullptr != pDevice)
    {
        // check if device changed
        if (pDevice->GetOpenALDevice() != m_device->GetOpenALDevice())
        {
            internal::Context* pContext = internal::Context::Create(*pDevice);

            if (nullptr != pContext)
            {
                pContext->MakeCurrent();

                std::shared_ptr<internal::BufferCollection> newBuffers = std::make_shared<internal::BufferCollection>();
                newBuffers->Initialize(config.bufferBatch);
                internal::BufferCollection::MigrationMapping bufferMapping = newBuffers->InheritCollection(*m_buffers);

                std::shared_ptr<internal::SourceCollection> newSources = std::make_shared<internal::SourceCollection>(*newBuffers);
                newSources->Initialize(config.sourceBatch);
                newSources->InheritCollection(
                    *m_sources.get()
                    , bufferMapping
                    , *m_context.get()
                    , *pContext
                );

                m_context->MakeCurrent();

                m_sources = newSources;
                m_buffers = newBuffers;

                m_context.reset(pContext);
                m_device.reset(pDevice);

                pContext->MakeCurrent();

                m_isInitialized = true;
            }
            else
            {
                LOG->Error("TulparAudio::Reinitialize() failed to initialize new context");
            }
        }
        else
        {
            LOG->Trace("TulparAudio::Reinitialize() device unchanged");

            m_buffers->Initialize(config.bufferBatch);
            m_sources->Initialize(config.sourceBatch);
        }
    }
    else
    {
        LOG->Error("TulparAudio::Reinitialize() failed to initialize new device");
    }

    if (false == m_isInitialized)
    {
        LOG->Error("TulparAudio::Reinitialize() failed");

        m_isInitialized = true;
        Deinitialize();
    }
    else
    {
        LOG->Debug("TulparAudio::Reinitialize() done");
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
        m_listener.reset();

        m_context.reset();
        m_device.reset();

        m_isInitialized = false;

        LOG->Debug("TulparAudio::Deinitialize() done");
    }
}

audio::Listener TulparAudio::GetListener() const
{
    assert(true == m_isInitialized);

    return m_listener->Get();
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
