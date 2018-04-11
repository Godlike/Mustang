/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_SOURCE_COLLECTION_HPP
#define TULPAR_INTERNAL_SOURCE_COLLECTION_HPP

#include <tulpar/internal/Collection.hpp>
#include <tulpar/internal/Context.hpp>

#include <tulpar/internal/BufferCollection.hpp>

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Source.hpp>

#include <tulpar/TulparAudio.hpp>

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

namespace tulpar
{
namespace internal
{

struct OpenAVSourceHandler
{
    static Collection<audio::Source>::Handles Generate(uint32_t batchSize);
    static void Reclaim(Collection<audio::Source>::Handle handle);
    static void Delete(Collection<audio::Source>::Handles const& handle);
};

class SourceCollection
    : public Collection<audio::Source>
{
public:
    using SourceHandle = audio::Source::Handle;
    using BufferHandle = audio::Buffer::Handle;

    SourceCollection(BufferCollection const& buffers
        , Collection<audio::Source>::HandleGenerator generator = OpenAVSourceHandler::Generate
        , Collection<audio::Source>::HandleReclaimer reclaimer = OpenAVSourceHandler::Reclaim
        , Collection<audio::Source>::HandleDeleter deleter = OpenAVSourceHandler::Delete
    );

    virtual ~SourceCollection();

    void InheritCollection(SourceCollection const& other
        , BufferCollection::MigrationMapping const& bufferMapping
        , Context& oldContext
        , Context& newContext
    );

    audio::Buffer GetSourceActiveBuffer(SourceHandle source) const;
    std::vector<audio::Buffer> GetSourceActiveBuffers(SourceHandle source) const;

    audio::Buffer GetSourceStaticBuffer(SourceHandle source) const;
    bool SetSourceStaticBuffer(SourceHandle source, BufferHandle buffer);

    std::vector<audio::Buffer> GetSourceQueuedBuffers(SourceHandle source) const;
    uint32_t GetSourceQueueIndex(SourceHandle source) const;
    bool QueueSourceBuffers(SourceHandle source, std::vector<audio::Buffer> const& buffers);

    bool ResetSource(SourceHandle source);

    bool PlaySource(SourceHandle source);
    bool StopSource(SourceHandle source);
    bool RewindSource(SourceHandle source);
    bool PauseSource(SourceHandle source);

    std::chrono::nanoseconds GetSourcePlaybackDuration(SourceHandle source) const;

    std::chrono::nanoseconds GetSourcePlaybackPosition(SourceHandle source) const;
    bool SetSourcePlaybackPosition(SourceHandle source, std::chrono::nanoseconds offset);

    float GetSourcePlaybackProgress(SourceHandle source) const;
    bool SetSourcePlaybackProgress(SourceHandle source, float value);

    audio::Source::State GetSourceState(SourceHandle source) const;
    audio::Source::Type GetSourceType(SourceHandle source) const;

    bool IsSourceRelative(SourceHandle source) const;
    bool SetSourceRelative(SourceHandle source, bool flag);

    bool IsSourceLooping(SourceHandle source) const;
    bool SetSourceLooping(SourceHandle source, bool flag);

    float GetSourcePitch(SourceHandle source) const;
    bool SetSourcePitch(SourceHandle source, float value);

    float GetSourceGain(SourceHandle source) const;
    bool SetSourceGain(SourceHandle source, float value);

    std::array<float, 3> GetSourcePosition(SourceHandle source) const;
    bool SetSourcePosition(SourceHandle source, std::array<float, 3> const& vec);

protected:
    virtual std::unique_ptr<audio::Source> CreateObject(SourceHandle source) override final;

private:
    struct Meta
    {
        uint32_t activeSampleCount                      = 0;
        std::chrono::nanoseconds activeTotalDuration    = std::chrono::nanoseconds{0};
    };

    BufferCollection const& m_buffers;

    std::unordered_map<SourceHandle, BufferHandle> m_sourceBuffers;
    std::unordered_map<SourceHandle, std::vector<BufferHandle>> m_sourceQueuedBuffers;
    std::unordered_map<SourceHandle, Meta> m_sourceMeta;
};

}
}

#endif // TULPAR_INTERNAL_SOURCE_COLLECTION_HPP
