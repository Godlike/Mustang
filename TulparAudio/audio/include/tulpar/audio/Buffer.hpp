/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_AUDIO_BUFFER_HPP
#define TULPAR_AUDIO_BUFFER_HPP

#include <mule/asset/Handler.hpp>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

namespace tulpar
{

namespace internal
{
class BufferCollection;
}

namespace audio
{

/** @brief  Proxy class for controlling audio buffer */
class Buffer
{
public:
    //! Shortcut to buffer handle type
    using Handle = uint32_t;

    /** @brief  Creates empty buffer object
     *
     *  Created empty object is invalid
     */
    Buffer();

    //! Default copy constructor
    Buffer(Buffer const& other) = default;

    //! Default assignment operator
    Buffer& operator=(Buffer const& other) = default;

    //! Default move constructor
    Buffer(Buffer&& other) = default;

    //! Default assignment-move operator
    Buffer& operator=(Buffer&& other) = default;

    //! Default destructor
    ~Buffer() = default;

    //! Returns shared pointer to underlying handle
    std::shared_ptr<Handle> GetSharedHandle() const { return m_handle; }

    /** @brief  Checks if object is valid and can be used
     *
     *  Validness is checked via Collection::IsValid() call on #m_pParent with #m_handle
     *
     *  @return @c true if object is valid, @c false otherwise
     */
    bool IsValid() const;

    /** @brief  Initializes buffer with given data
     *
     *  Apart from initializing buffer with audio data, parses and sets metadata
     *
     *  @param  asset   asset handle to audio content
     *
     *  @return @c true if data was set successfully, @c false otherwise
     */
    bool BindData(mule::asset::Handler asset);

    //! Returns name associated
    std::string GetDataName() const;

    //! Sets buffer name
    void SetDataName(std::string const& value);

    //! Returns channel count
    uint8_t GetChannelCount() const;

    //! Returns frequency
    uint32_t GetFrequencyHz() const;

    //! Returns sample count
    uint32_t GetSampleCount() const;

    //! Returns duration
    std::chrono::nanoseconds GetDuration() const;

    //! Resets given buffer
    void Reset();

private:
    friend class internal::BufferCollection;

    /** @brief  Creates buffer object
     *
     *  @param  handle  handle indicating underlying buffer object
     *  @param  parent  parent collection
     */
    Buffer(std::shared_ptr<Handle> pHandle
        , std::shared_ptr<internal::BufferCollection*> pParent);

    //! Parent collection
    std::shared_ptr<internal::BufferCollection*> m_pParent;

    //! Underlying handle identifying buffer object
    std::shared_ptr<Handle> m_handle;
};

}
}

#endif // TULPAR_AUDIO_BUFFER_HPP
