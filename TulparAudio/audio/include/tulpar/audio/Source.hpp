/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_AUDIO_SOURCE_HPP
#define TULPAR_AUDIO_SOURCE_HPP

#include <tulpar/audio/Buffer.hpp>

#include <cstdint>
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
    void BindBuffer(Buffer buffer);

    bool Reset();

private:
    friend class internal::SourceCollection;

    Source(Handle handle, std::weak_ptr<internal::SourceCollection> parent);

    std::weak_ptr<internal::SourceCollection> m_parent;

    Handle m_handle;
};

}
}

#endif // TULPAR_AUDIO_SOURCE_HPP
