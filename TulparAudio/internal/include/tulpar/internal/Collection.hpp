/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_COLLECTION_HPP
#define TULPAR_INTERNAL_COLLECTION_HPP

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Source.hpp>

#include <mule/asset/Content.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace tulpar
{
namespace internal
{

template<typename T>
    class Collection
{
public:
    Collection();

    Collection(Collection const& other) = delete;
    Collection& operator=(Collection const& other) = delete;

    virtual ~Collection() = default;

    void Initialize(uint32_t batchSize);

    T Get(typename T::Handle handle) const;
    bool IsValid(typename T::Handle handle) const;
    T Spawn();
    void Reclaim(typename T::Handle handle);

protected:
    virtual void GenerateHandles(uint32_t batchSize) = 0;
    virtual T* GenerateObject(typename T::Handle handle) const = 0;
    virtual T* CreateObject(typename T::Handle handle) = 0;

    uint32_t m_batchSize;

    std::unordered_map<typename T::Handle, T*> m_sources;

    std::vector<typename T::Handle> m_used;
    std::queue<typename T::Handle> m_available;
};

class BufferCollection
    : public Collection<audio::Buffer>
    , public std::enable_shared_from_this<BufferCollection>
{
public:
    using Handle = audio::Buffer::Handle;

    BufferCollection();
    virtual ~BufferCollection();

    void SetBufferName(Handle handle, std::string const& name);
    std::string GetBufferName(Handle handle) const;

    bool SetBufferData(Handle handle, mule::asset::Content const& content);

    bool ResetBuffer(Handle handle);

protected:
    virtual void GenerateHandles(uint32_t batchSize) override final;
    virtual audio::Buffer* GenerateObject(Handle handle) const override final;
    virtual audio::Buffer* CreateObject(Handle handle) override final;

private:
    std::unordered_map<Handle, std::string> m_bufferNames;
};

class SourceCollection
    : public Collection<audio::Source>
    , public std::enable_shared_from_this<SourceCollection>
{
public:
    using SourceHandle = audio::Source::Handle;
    using BufferHandle = audio::Buffer::Handle;

    SourceCollection(BufferCollection const& buffers);
    virtual ~SourceCollection();

    void BindBuffer(SourceHandle source, BufferHandle buffer);
    audio::Buffer GetSourceBuffer(SourceHandle source) const;

    bool ResetSource(SourceHandle source);

protected:
    virtual void GenerateHandles(uint32_t batchSize) override final;
    virtual audio::Source* GenerateObject(SourceHandle handle) const override final;
    virtual audio::Source* CreateObject(SourceHandle source) override final;

private:
    BufferCollection const& m_buffers;

    std::unordered_map<SourceHandle, BufferHandle> m_sourceBuffers;
};

}
}

#include <tulpar/internal/Collection.imp>

#endif // TULPAR_INTERNAL_COLLECTION_HPP
