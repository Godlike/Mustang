/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_AUDIO_SOURCE_HPP
#define TULPAR_AUDIO_SOURCE_HPP

#include <tulpar/audio/Buffer.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

namespace tulpar
{

namespace internal
{
class SourceCollection;
}

namespace audio
{

/** @brief  Proxy class for controlling audio source */
class Source
{
public:
    //! Shortcut to source handle type
    using Handle = uint32_t;

    //! Source state enumeration
    enum class State : uint8_t
    {
        Unknown = 0x00  /**< Fallback value indicating something went wrong */

        , Initial   /**< Initial state, buffers can be set */
        , Playing   /**< Playback in progress */
        , Paused    /**< Playback is paused and can be continued from paused offset */
        , Stopped   /**< Playback is stopped, buffers can be set */
    };

    //! Source type enumeration
    enum class Type : uint8_t
    {
        Unknown = 0x00  /**< Fallback value indicating something went wrong */

        , Undetermined  /**< Source buffer is unset */
        , Static        /**< Source buffer is static */
        , Streaming     /**< Source has a queue of buffers */
    };

    /** @brief  Creates empty source object
     *
     *  Created empty object is invalid
     */
    Source();

    //! Default copy constructor
    Source(Source const& other) = default;

    //! Default assignment operator
    Source& operator=(Source const& other) = default;

    //! Default move constructor
    Source(Source&& other) = default;

    //! Default assignment-move operator
    Source& operator=(Source&& other) = default;

    //! Default destructor
    ~Source() = default;

    //! Returns shared pointer to underlying handle
    std::shared_ptr<Handle> GetSharedHandle() const { return m_handle; }

    /** @brief  Checks if object is valid and can be used
     *
     *  Validness is checked via Collection::IsValid() call on #m_pParent with #m_handle
     *
     *  @return @c true if object is valid, @c false otherwise
     */
    bool IsValid() const;

    /** @brief  Returns active buffer object
     *
     *  If source type is static, returns associated buffer
     *  If source type is streaming, returns a buffer that's used right now
     *
     *  @return buffer object
     */
    Buffer GetBuffer() const;

    /** @brief  Resets associated buffer object
     *
     *  @sa SetStaticBuffer, QueueBuffers
     */
    void ResetBuffer();

    //! Returns associated static buffer object
    Buffer GetStaticBuffer() const;

    //! Sets static buffer object
    bool SetStaticBuffer(Buffer buffer);

    //! Returns a vector of all queued buffers
    std::vector<Buffer> GetQueuedBuffers() const;

    //! Returns index of currently used buffer in queue
    uint32_t GetQueueIndex() const;

    /** @brief  Queues given collection of audio buffers
     *
     *  @param  buffers collection of buffers
     *
     *  @return @c true if buffers were queued successfully, @c false otherwise
     */
    bool QueueBuffers(std::vector<Buffer> const& buffers);

    /** @brief  Resets source object
     *
     *  Stops any activities with the source, resets associated buffers and
     *  removes all stored meta information
     */
    void Reset();

    /** @brief  Starts playing associated buffers
     *
     *  Changes state to audio::Source::Playing. When playback is complete,
     *  state is changed to audio::Source::Stopped unless source is looping
     *
     *  @note   to play a source it should have at least one valid buffer
     *
     *  @return @c true if source is now playing, @c false otherwise
     *
     *  @sa IsLooping, SetLooping
     */
    bool Play();

    /** @brief  Stops playback
     *
     *  Changes state to audio::Source::Stopped
     *  After calling Stop(), SetStaticBuffer() and QueueBuffers() can be used
     *  to change associated buffers
     *
     *  @note   Even though OpenAL specification does not say that playback
     *          offset is reset to 0, implementations do that regardless
     *
     *  @return @c true if source was stopped, @c false otherwise
     */
    bool Stop();

    /** @brief  Stops playback and rewinds source back to start
     *
     *  Stops the source, resets progress and changes its state back to
     *  audio::Source::Initial
     *  After calling Rewind(), SetStaticBuffer() and QueueBuffers() can be
     *  used to change associated buffers
     *
     *  @return @c true if source was rewound, @c false otherwise
     */
    bool Rewind();

    /** @brief  Pauses playback
     *
     *  @return @c true if source is now paused, @c false otherwise
     */
    bool Pause();

    //! Returns playback queue duration
    std::chrono::nanoseconds GetPlaybackDuration() const;

    //! Returns current playback position
    std::chrono::nanoseconds GetPlaybackPosition() const;

    //! Sets playback position
    bool SetPlaybackPosition(std::chrono::nanoseconds offset);

    //! Returns playback progress
    float GetPlaybackProgress() const;

    //! Sets playback progress
    bool SetPlaybackProgress(float value);

    //! Returns source state
    State GetState() const;

    //! Returns source type
    Type GetType() const;

    /** @brief  Returns relativeness flag
     *
     *  If source is relative then its position is treated as relative to that
     *  of a listener, otherwise it is treated as absolute
     *
     *  @return @c true if source position is relative, @c false otherwise
     */
    bool IsRelative() const;

    /** @brief  Sets relativeness flag
     *
     *  If source is relative then its position is treated as relative to that
     *  of a listener, otherwise it is treated as absolute
     *
     *  @param  flag    value to be set
     *
     *  @return @c true if relativeness flag was set, @c false otherwise
     */
    bool SetRelative(bool flag);

    /** @brief  Returns looping flag
     *
     *  If source is looping then instead of going to audio::Source::Stopped
     *  state when playback is finished, it would instead go back to
     *  audio::Source::Initial and start playback from the start
     *
     *  @return @c true if source is looping, @c false otherwise
     */
    bool IsLooping() const;

    /** @brief  Sets looping flag
     *
     *  If source is looping then instead of going to audio::Source::Stopped
     *  state when playback is finished, it would instead go back to
     *  audio::Source::Initial and start playback from the start
     *
     *  @param  flag    value to be set
     *
     *  @return @c true if looping flag was set, @c false otherwise
     */
    bool SetLooping(bool flag);

    //! Returns pitch value
    float GetPitch() const;

    //! Sets pitch value
    bool SetPitch(float value);

    //! Returns gain value
    float GetGain() const;

    //! Sets gain value
    bool SetGain(float value);

    //! Returns position
    std::array<float, 3> GetPosition() const;

    //! Sets position
    bool SetPosition(std::array<float, 3> vec);

private:
    friend class internal::SourceCollection;

    /** @brief  Creates source object
     *
     *  @param  handle  handle indicating underlying source object
     *  @param  parent  parent collection
     */
    Source(std::shared_ptr<Handle> handle
        , std::shared_ptr<internal::SourceCollection*> parent);

    //! Parent collection
    std::shared_ptr<internal::SourceCollection*> m_pParent;

    //! Underlying handle identifying source object
    std::shared_ptr<Handle> m_handle;
};

//! String representation ostream overload for source state
std::ostream& operator<<(std::ostream& os, Source::State const& state);

//! String representation ostream overload for source type
std::ostream& operator<<(std::ostream& os, Source::Type const& type);

}
}

#endif // TULPAR_AUDIO_SOURCE_HPP
