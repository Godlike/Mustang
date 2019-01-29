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
#include <unordered_map>
#include <vector>

namespace tulpar
{
namespace internal
{

/** @brief  Contains basic source id operations */
namespace OpenAVSourceHandler
{
    /** @brief  Generates a collection of source handles via OpenAL
     *
     *  @param  batchSize   number of handles to generate
     *
     *  @return a collection of newly generated source handles
     */
    Collection<audio::Source>::Handles Generate(uint32_t batchSize);

    /** @brief  Resets given handle
     *
     *  Stops given source
     *
     *  @param  handle  valid source handle
     */
    void Reclaim(Collection<audio::Source>::Handle handle);

    /** @brief  Returns given collection of handles to OpenAL
     *
     *  @param  handle  collection of valid source handles
     */
    void Delete(Collection<audio::Source>::Handles const& handle);
}

/** @brief  Collection working with audio sources */
class SourceCollection
    : public Collection<audio::Source>
{
public:
    //! Shortcut to source handle type
    using SourceHandle = audio::Source::Handle;

    //! Shortcut to buffer handle type
    using BufferHandle = audio::Buffer::Handle;

    /** @brief  Constructs source collection object
     *
     *  @param  buffers     a buffer collection that should be used for
     *                      buffer handles
     *  @param  generator   functor that will be called when generating
     *                      new handle
     *  @param  reclaimer   functor that will be called when reclaiming
     *                      previously used handle
     *  @param  deleter     functor that will be called when releasing
     *                      previously used handle
     */
    SourceCollection(BufferCollection const& buffers
        , Collection<audio::Source>::HandleGenerator generator = OpenAVSourceHandler::Generate
        , Collection<audio::Source>::HandleReclaimer reclaimer = OpenAVSourceHandler::Reclaim
        , Collection<audio::Source>::HandleDeleter deleter = OpenAVSourceHandler::Delete
    );

    /** @brief  Destructs source collection */
    virtual ~SourceCollection() = default;

    /** @brief  Migrates sources from given collection
     *
     *  Uses @p bufferMapping to map buffer handles from @p other to buffer
     *  handles from @p m_buffers
     *
     *  @p oldContext is used to gather source infromation from @p other
     *  @p newContext is used to generate and fill source information in this
     *  collection
     *
     *  @attention  all Source objects from @p other are updated to point to
     *              this collection with new handles
     *
     *  @param  other           collection to be copied
     *  @param  bufferMapping   buffer handle mapping
     *  @param  oldContext      old context compatible with @p other
     *  @param  newContext      new context compatible with this collection
     */
    void InheritCollection(SourceCollection const& other
        , BufferCollection::MigrationMapping const& bufferMapping
        , Context& oldContext
        , Context& newContext
    );

    /** @brief  Returns active buffer object associated with given source
     *
     *  If @p source type is static, returns associated buffer
     *  If @p source type is streaming, returns a buffer that's used right now
     *
     *  @param  source  valid handle
     *
     *  @return buffer object
     */
    audio::Buffer GetSourceActiveBuffer(SourceHandle source) const;

    /** @brief  Returns a collection of buffers associated with given source
     *
     *  @param  source  valid handle
     *
     *  @return a collection of buffer objects
     */
    std::vector<audio::Buffer> GetSourceActiveBuffers(SourceHandle source) const;

    //! Returns associated static buffer with given source
    audio::Buffer GetSourceStaticBuffer(SourceHandle source) const;

    /** @brief  Sets static buffer for given source
     *
     *  @note   empty buffer handle (audio::Buffer()) can be used to reset source
     *
     *  @param  source  valid source handle
     *  @param  buffer  buffer handle
     *
     *  @return @c true if buffer was set successfully, @c false otherwise
     */
    bool SetSourceStaticBuffer(SourceHandle source, BufferHandle buffer);

    //! Returns a vector of all queued buffers for given source
    std::vector<audio::Buffer> GetSourceQueuedBuffers(SourceHandle source) const;

    //! Returns index of currently used buffer in queue for given source
    uint32_t GetSourceQueueIndex(SourceHandle source) const;

    /** @brief  Queues given collection of audio buffers for given source
     *
     *  @param  source  valid source handle
     *  @param  buffers collection of buffers
     *
     *  @return @c true if buffers were queued successfully, @c false otherwise
     */
    bool QueueSourceBuffers(SourceHandle source, std::vector<audio::Buffer> const& buffers);

    /** @brief  Resets given source
     *
     *  Stops any activities with the source, resets associated buffers and
     *  removes all stored meta information
     *
     *  @param  source  valid source handle
     */
    void ResetSource(SourceHandle source);

    /** @brief  Starts playing given source
     *
     *  Changes source state to audio::Source::Playing. When playback
     *  is complete, source state is changed to audio::Source::Stopped
     *  unless current source is looping
     *
     *  @note   to play a source it should have at least one valid buffer
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source is now playing, @c false otherwise
     *
     *  @sa IsSourceLooping, SetSourceLooping
     */
    bool PlaySource(SourceHandle source);

    /** @brief  Stops playing given source
     *
     *  Changes source state to audio::Source::Stopped
     *  After calling StopSource(), SetSourceStaticBuffer() and
     *  QueueSourceBuffers() can be used to change associated buffers
     *
     *  @note   Even though OpenAL specification does not say that playback
     *          offset is reset to 0, implementations do that regardless
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source was stopped, @c false otherwise
     */
    bool StopSource(SourceHandle source);

    /** @brief  Stops playing given source and rewinds it back to start
     *
     *  Stops the source, resets progress and changes its state back to
     *  audio::Source::Initial
     *  After calling RewindSource(), SetSourceStaticBuffer() and
     *  QueueSourceBuffers() can be used to change associated buffers
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source was rewound, @c false otherwise
     */
    bool RewindSource(SourceHandle source);

    /** @brief  Pauses given source playback
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source is now paused, @c false otherwise
     */
    bool PauseSource(SourceHandle source);

    //! Returns playback queue duration for given source
    std::chrono::nanoseconds GetSourcePlaybackDuration(SourceHandle source) const;

    //! Returns current playback position for given source
    std::chrono::nanoseconds GetSourcePlaybackPosition(SourceHandle source) const;

    //! Sets playback position for given source
    bool SetSourcePlaybackPosition(SourceHandle source, std::chrono::nanoseconds offset);

    //! Returns playback progress for given source
    float GetSourcePlaybackProgress(SourceHandle source) const;

    //! Sets playback progress for given source
    bool SetSourcePlaybackProgress(SourceHandle source, float value);

    //! Returns state of given source
    audio::Source::State GetSourceState(SourceHandle source) const;

    //! Returns type of given source
    audio::Source::Type GetSourceType(SourceHandle source) const;

    /** @brief  Returns relativeness flag for given source
     *
     *  If source is relative then its position is treated as relative to that
     *  of a listener, otherwise it is treated as absolute
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source position is relative, @c false otherwise
     */
    bool IsSourceRelative(SourceHandle source) const;

    /** @brief  Sets relativeness flag for given source
     *
     *  If source is relative then its position is treated as relative to that
     *  of a listener, otherwise it is treated as absolute
     *
     *  @param  source  valid source handle
     *  @param  flag    value to be set
     *
     *  @return @c true if relativeness flag was set, @c false otherwise
     */
    bool SetSourceRelative(SourceHandle source, bool flag);

    /** @brief  Returns looping flag for given source
     *
     *  If source is looping then instead of going to audio::Source::Stopped
     *  state when playback is finished, it would instead go back to
     *  audio::Source::Initial and start playback from the start
     *
     *  @param  source  valid source handle
     *
     *  @return @c true if source is looping, @c false otherwise
     */
    bool IsSourceLooping(SourceHandle source) const;

    /** @brief  Sets looping flag for given source
     *
     *  If source is looping then instead of going to audio::Source::Stopped
     *  state when playback is finished, it would instead go back to
     *  audio::Source::Initial and start playback from the start
     *
     *  @param  source  valid source handle
     *  @param  flag    value to be set
     *
     *  @return @c true if looping flag was set, @c false otherwise
     */
    bool SetSourceLooping(SourceHandle source, bool flag);

    //! Returns pitch value for given source
    float GetSourcePitch(SourceHandle source) const;

    //! Sets pitch value for given source
    bool SetSourcePitch(SourceHandle source, float value);

    //! Returns gain value for given source
    float GetSourceGain(SourceHandle source) const;

    //! Sets gain value for given source
    bool SetSourceGain(SourceHandle source, float value);

    //! Returns position of given source
    std::array<float, 3> GetSourcePosition(SourceHandle source) const;

    //! Sets position for given source
    bool SetSourcePosition(SourceHandle source, std::array<float, 3> const& vec);

protected:
    //! Creates a source object associated with this collection and given handle
    virtual audio::Source CreateObject(SourceHandle source) override final;

private:
    //! Meta information for initialized source handles
    struct Meta
    {
        uint32_t activeSampleCount                      = 0;
        std::chrono::nanoseconds activeTotalDuration    = std::chrono::nanoseconds{0};
    };

    //! Buffer collection that process provided buffer handles
    BufferCollection const& m_buffers;

    //! Collection of static buffer handles associated with sources
    std::unordered_map<SourceHandle, BufferHandle> m_sourceBuffers;

    //! Collection of queued buffer handles associated with sources
    std::unordered_map<SourceHandle, std::vector<BufferHandle>> m_sourceQueuedBuffers;

    //! Collection of meta information for sources
    std::unordered_map<SourceHandle, Meta> m_sourceMeta;
};

}
}

#endif // TULPAR_INTERNAL_SOURCE_COLLECTION_HPP
