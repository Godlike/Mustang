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

class Buffer
{
public:
    using Handle = uint32_t;

    Buffer();

    Buffer(Buffer const& other) = default;
    Buffer& operator=(Buffer const& other) = default;

    Buffer(Buffer&& other) = default;
    Buffer& operator=(Buffer&& other) = default;

    ~Buffer() = default;

    Handle GetHandle() const { return m_handle; }
    operator Handle() const { return m_handle; }

    bool IsValid() const;

    bool BindData(mule::asset::Handler asset);

    std::string GetDataName() const;
    void SetDataName(std::string const& value);

    uint8_t GetChannelCount() const;
    uint32_t GetFrequencyHz() const;
    uint32_t GetSampleCount() const;
    std::chrono::nanoseconds GetDuration() const;

    bool Reset();

private:
    friend class internal::BufferCollection;

    Buffer(Handle handle, std::weak_ptr<internal::BufferCollection> parent);

    std::weak_ptr<internal::BufferCollection> m_parent;

    Handle m_handle;
};

}
}

#endif // TULPAR_AUDIO_BUFFER_HPP
