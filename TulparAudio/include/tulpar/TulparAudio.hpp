/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_TULPAR_AUDIO_HPP
#define TULPAR_TULPAR_AUDIO_HPP

#include <tulpar/TulparConfigurator.hpp>

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Listener.hpp>
#include <tulpar/audio/Source.hpp>

#include <mule/asset/Handler.hpp>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace tulpar
{

namespace internal
{
class BufferCollection;
class Context;
class Device;
class ListenerController;
class SourceCollection;
}

/** @brief  Tulpar library entry point */
class TulparAudio
{
public:
    TulparAudio();

    TulparAudio(TulparAudio const& other) = delete;
    TulparAudio& operator=(TulparAudio const& other) = delete;

    ~TulparAudio();

    bool Initialize(TulparConfigurator const& config);
    bool Reinitialize(TulparConfigurator const& config);
    void Deinitialize();

    audio::Listener GetListener() const;

    audio::Source GetSource(audio::Source::Handle handle) const;
    audio::Source SpawnSource();

    audio::Buffer GetBuffer(audio::Buffer::Handle handle) const;
    audio::Buffer SpawnBuffer();

private:
    bool m_isInitialized;

    std::shared_ptr<internal::Device> m_device;
    std::shared_ptr<internal::Context> m_context;
    std::shared_ptr<internal::ListenerController> m_listener;
    std::shared_ptr<internal::BufferCollection> m_buffers;
    std::shared_ptr<internal::SourceCollection> m_sources;
};

}

#endif // TULPAR_TULPAR_AUDIO_HPP
