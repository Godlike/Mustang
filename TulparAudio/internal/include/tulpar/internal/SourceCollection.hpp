/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_SOURCE_COLLECTION_HPP
#define TULPAR_INTERNAL_SOURCE_COLLECTION_HPP

#include <tulpar/internal/Collection.hpp>

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Source.hpp>

#include <memory>

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

class BufferCollection;

class SourceCollection
    : public Collection<audio::Source>
    , public std::enable_shared_from_this<SourceCollection>
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

    void BindBuffer(SourceHandle source, BufferHandle buffer);
    audio::Buffer GetSourceBuffer(SourceHandle source) const;

    bool ResetSource(SourceHandle source);

    bool PlaySource(SourceHandle source);
    bool StopSource(SourceHandle source);
    bool RewindSource(SourceHandle source);
    bool PauseSource(SourceHandle source);

protected:
    virtual audio::Source* GenerateObject(SourceHandle handle) const override final;
    virtual audio::Source* CreateObject(SourceHandle source) override final;

private:
    BufferCollection const& m_buffers;

    std::unordered_map<SourceHandle, BufferHandle> m_sourceBuffers;
};

}
}

#endif // TULPAR_INTERNAL_SOURCE_COLLECTION_HPP
