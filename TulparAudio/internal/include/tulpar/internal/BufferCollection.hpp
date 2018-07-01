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
#include <string>
#include <unordered_map>

namespace tulpar
{
namespace internal
{

/** @brief  Handles basic buffer id operations */
struct OpenAVBufferHandler
{
    /** @brief  Generates a collection of buffer handles via OpenAL
     *
     *  Underlying buffer objects registered with OpenAL are tied to device
     *  object that is active at the moment of calling this method
     *
     *  @param  batchSize   number of handles to generate
     *
     *  @return a collection of newly generated buffer handles
     */
    static Collection<audio::Buffer>::Handles Generate(uint32_t batchSize);

    /** @brief  Resets given handle
     *
     *  @note   Does nothing at the moment
     *
     *  @param  handle  valid buffer handle
     */
    static void Reclaim(Collection<audio::Buffer>::Handle handle);

    /** @brief  Returns given collection of handles to OpenAL
     *
     *  @param  handle  collection of valid buffer handles
     */
    static void Delete(Collection<audio::Buffer>::Handles const& handles);
};

/** @brief  Collection working with audio buffers */
class BufferCollection
    : public Collection<audio::Buffer>
{
public:
    //! Shortcut to buffer handle type
    using Handle = audio::Buffer::Handle;

    //! Shortcut to buffer migration mapping as hashmap<old, new>
    using MigrationMapping = std::unordered_map<audio::Buffer::Handle, audio::Buffer::Handle>;

    /** @brief  Constructs buffer collection object
     *
     *  @param  generator   functor that will be called when generating
     *                      new handle
     *  @param  reclaimer   functor that will be called when reclaiming
     *                      previously used handle
     *  @param  deleter     functor that will be called when releasing
     *                      previously used handle
     */
    BufferCollection(
        Collection<audio::Buffer>::HandleGenerator generator = OpenAVBufferHandler::Generate
        , Collection<audio::Buffer>::HandleReclaimer reclaimer = OpenAVBufferHandler::Reclaim
        , Collection<audio::Buffer>::HandleDeleter deleter = OpenAVBufferHandler::Delete
    );

    /** @brief  Destructs buffer collection */
    virtual ~BufferCollection();

    /** @brief  Migrates buffers from given collection
     *
     *  @note   Context::MakeCurrent() has to be called on new context prior to
     *          calling this method in order to migrate buffers
     *
     *  @attention  all Buffer objects from @p other are updated to point to
     *              this collection with new handles
     *
     *  @param  other   collection to be copied
     *
     *  @return buffer handle mapping
     */
    MigrationMapping InheritCollection(BufferCollection const& other);

    //! Returns buffer name
    std::string GetBufferName(Handle handle) const;

    //! Sets buffer name
    void SetBufferName(Handle handle, std::string const& name);

    //! Returns buffer channel count
    uint8_t GetBufferChannelCount(Handle handle) const;

    //! Returns buffer frequency
    uint32_t GetBufferFrequencyHz(Handle handle) const;

    //! Returns buffer sample count
    uint32_t GetBufferSampleCount(Handle handle) const;

    //! Returns buffer duration
    std::chrono::nanoseconds GetBufferDuration(Handle handle) const;

    /** @brief  Initializes buffer with given data
     *
     *  Apart from initializing buffer with audio data, parses and sets
     *  metadata, see @ref BufferInfo for more details.
     *
     *  @param  handle  valid buffer handle
     *  @param  asset   asset handle to audio content
     *
     *  @return @c true if data was set successfully, @c false otherwise
     */
    bool SetBufferData(Handle handle, mule::asset::Handler asset);

    //! Resets given buffer
    void ResetBuffer(Handle handle);

protected:
    //! Creates a buffer object associated with this collection and given handle
    virtual audio::Buffer CreateObject(Handle handle) override final;

private:
    //! Meta information for initialized buffer handles
    struct BufferInfo
    {
        //! Handle to associated audio content
        mule::asset::Handler asset          = mule::asset::Handler();

        //! Buffer name
        std::string name                    = std::string();

        //! Number of audio channels
        uint8_t channels                    = 0;

        //! Buffer frequency in hz
        uint32_t frequencyHz                = 0;

        //! Total sample count
        uint32_t sampleCount                = 0;

        //! Total duration
        std::chrono::nanoseconds duration   = std::chrono::nanoseconds{0};
    };

    //! Collection of meta information for buffers
    std::unordered_map<Handle, BufferInfo> m_bufferInfo;
};

}
}

#endif // TULPAR_INTERNAL_BUFFER_COLLECTION_HPP
