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

class Source
{
public:
    using Handle = uint32_t;

    enum class State : uint8_t
    {
        Unknown = 0x00

        , Initial
        , Playing
        , Paused
        , Stopped
    };

    enum class Type : uint8_t
    {
        Unknown = 0x00

        , Undetermined
        , Static
        , Streaming
    };

    Source();

    Source(Source const& other) = default;
    Source& operator=(Source const& other) = default;

    Source(Source&& other) = default;
    Source& operator=(Source&& other) = default;

    ~Source() = default;

    Handle GetHandle() const { return m_handle; }
    operator Handle() const { return m_handle; }

    bool IsValid() const;

    Buffer GetBuffer() const;
    void ResetBuffer();

    Buffer GetStaticBuffer() const;
    bool SetStaticBuffer(Buffer buffer);

    std::vector<Buffer> GetQueuedBuffers() const;
    uint32_t GetQueueIndex() const;
    bool QueueBuffers(std::vector<Buffer> const& buffers);

    bool Reset();

    bool Play();
    bool Stop();
    bool Rewind();
    bool Pause();

    std::chrono::nanoseconds GetPlaybackPosition() const;
    bool SetPlaybackPosition(std::chrono::nanoseconds offset);

    float GetPlaybackProgress() const;
    bool SetPlaybackProgress(float value);

    State GetState() const;
    Type GetType() const;

    bool IsRelative() const;
    bool SetRelative(bool flag);

    bool IsLooping() const;
    bool SetLooping(bool flag);

    float GetPitch() const;
    bool SetPitch(float value);

    float GetGain() const;
    bool SetGain(float value);

    std::array<float, 3> GetPosition() const;
    bool SetPosition(std::array<float, 3> vec);

private:
    friend class internal::SourceCollection;

    Source(Handle handle, std::weak_ptr<internal::SourceCollection> parent);

    std::weak_ptr<internal::SourceCollection> m_parent;

    Handle m_handle;
};

std::ostream& operator<<(std::ostream& os, Source::State const& state);
std::ostream& operator<<(std::ostream& os, Source::Type const& type);

}
}

#endif // TULPAR_AUDIO_SOURCE_HPP
