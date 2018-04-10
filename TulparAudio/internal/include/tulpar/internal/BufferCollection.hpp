/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_BUFFER_COLLECTION_HPP
#define TULPAR_INTERNAL_BUFFER_COLLECTION_HPP

#include <tulpar/internal/Collection.hpp>

#include <tulpar/audio/Buffer.hpp>

#include <tulpar/TulparAudio.hpp>

#include <mule/asset/Handler.hpp>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

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
{
public:
    using Handle = audio::Buffer::Handle;
    using MigrationMapping = std::unordered_map<audio::Buffer::Handle, audio::Buffer::Handle>;

    BufferCollection(
        Collection<audio::Buffer>::HandleGenerator generator = OpenAVBufferHandler::Generate
        , Collection<audio::Buffer>::HandleReclaimer reclaimer = OpenAVBufferHandler::Reclaim
        , Collection<audio::Buffer>::HandleDeleter deleter = OpenAVBufferHandler::Delete
    );

    virtual ~BufferCollection();

    MigrationMapping InheritCollection(BufferCollection const& other);

    std::string GetBufferName(Handle handle) const;
    void SetBufferName(Handle handle, std::string const& name);

    uint8_t GetBufferChannelCount(Handle handle) const;
    uint32_t GetBufferFrequencyHz(Handle handle) const;
    uint32_t GetBufferSampleCount(Handle handle) const;
    std::chrono::nanoseconds GetBufferDuration(Handle handle) const;

    bool SetBufferData(Handle handle, mule::asset::Handler asset);

    bool ResetBuffer(Handle handle);

    uint32_t GetFrequency(Handle handle) const;

protected:
    virtual std::unique_ptr<audio::Buffer> GenerateObject(Handle handle) const override final;
    virtual std::unique_ptr<audio::Buffer> CreateObject(Handle handle) override final;

private:
    struct BufferInfo
    {
        mule::asset::Handler asset          = mule::asset::Handler();
        std::string name                    = std::string();
        uint8_t channels                    = 0;
        uint32_t frequencyHz                = 0;
        uint32_t sampleCount                = 0;
        std::chrono::nanoseconds duration   = std::chrono::nanoseconds{0};
    };

    std::unordered_map<Handle, BufferInfo> m_bufferInfo;
};

}
}

#endif // TULPAR_INTERNAL_BUFFER_COLLECTION_HPP
