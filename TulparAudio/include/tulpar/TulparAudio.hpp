/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_TULPAR_AUDIO_HPP
#define TULPAR_TULPAR_AUDIO_HPP

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Source.hpp>

#include <mule/asset/Handler.hpp>

#include <vector>
#include <memory>

namespace tulpar
{

namespace internal
{
class BufferCollection;
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

    void Initialize(uint32_t bufferBatch = 32, uint32_t sourceBatch = 32);
    void Deinitialize();

    audio::Source GetSource(audio::Source::Handle handle) const;
    audio::Source SpawnSource();

    audio::Buffer GetBuffer(audio::Buffer::Handle handle) const;
    audio::Buffer SpawnBuffer();

private:
    bool m_isInitialized;

    std::shared_ptr<internal::BufferCollection> m_buffers;
    std::shared_ptr<internal::SourceCollection> m_sources;
};

}

#endif // TULPAR_TULPAR_AUDIO_HPP
