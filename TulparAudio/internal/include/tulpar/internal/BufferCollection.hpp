/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_BUFFER_COLLECTION_HPP
#define TULPAR_INTERNAL_BUFFER_COLLECTION_HPP

#include <tulpar/internal/Collection.hpp>

#include <tulpar/audio/Buffer.hpp>

#include <mule/asset/Content.hpp>

#include <memory>

namespace tulpar
{
namespace internal
{

struct OpenAVBufferHandler
{
    static Collection<audio::Buffer>::Handles Generate(uint32_t batchSize);
    static void Reclaim(Collection<audio::Buffer>::Handle handle);
    static void Delete(Collection<audio::Buffer>::Handles const& handles);
};

class BufferCollection
    : public Collection<audio::Buffer>
    , public std::enable_shared_from_this<BufferCollection>
{
public:
    using Handle = audio::Buffer::Handle;

    BufferCollection(
        Collection<audio::Buffer>::HandleGenerator generator = OpenAVBufferHandler::Generate
        , Collection<audio::Buffer>::HandleReclaimer reclaimer = OpenAVBufferHandler::Reclaim
        , Collection<audio::Buffer>::HandleDeleter deleter = OpenAVBufferHandler::Delete
    );

    virtual ~BufferCollection();

    void SetBufferName(Handle handle, std::string const& name);
    std::string GetBufferName(Handle handle) const;

    bool SetBufferData(Handle handle, mule::asset::Content const& content);

    bool ResetBuffer(Handle handle);

protected:
    virtual audio::Buffer* GenerateObject(Handle handle) const override final;
    virtual audio::Buffer* CreateObject(Handle handle) override final;

private:
    std::unordered_map<Handle, std::string> m_bufferNames;
};

}
}

#endif // TULPAR_INTERNAL_BUFFER_COLLECTION_HPP
