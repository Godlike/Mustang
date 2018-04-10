/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/audio/Buffer.hpp>

#include <tulpar/internal/BufferCollection.hpp>

namespace tulpar
{
namespace audio
{

Buffer::Buffer()
    : m_pParent(nullptr)
    , m_handle(std::make_shared<Handle>(0))
{

}

bool Buffer::IsValid() const
{
    return (nullptr != m_handle.get())
        && (nullptr != m_pParent.get())
        && (*m_pParent)->IsValid(*m_handle);
}

bool Buffer::BindData(mule::asset::Handler asset)
{
    assert(IsValid());

    return (*m_pParent)->SetBufferData(*m_handle, asset);
}

std::string Buffer::GetDataName() const
{
    assert(IsValid());

    return (*m_pParent)->GetBufferName(*m_handle);
}

void Buffer::SetDataName(std::string const& value)
{
    assert(IsValid());

    (*m_pParent)->SetBufferName(*m_handle, value);
}

uint8_t Buffer::GetChannelCount() const
{
    assert(IsValid());

    return (*m_pParent)->GetBufferChannelCount(*m_handle);
}

uint32_t Buffer::GetFrequencyHz() const
{
    assert(IsValid());

    return (*m_pParent)->GetBufferFrequencyHz(*m_handle);
}

uint32_t Buffer::GetSampleCount() const
{
    assert(IsValid());

    return (*m_pParent)->GetBufferSampleCount(*m_handle);
}

std::chrono::nanoseconds Buffer::GetDuration() const
{
    assert(IsValid());

    return (*m_pParent)->GetBufferDuration(*m_handle);
}

bool Buffer::Reset()
{
    assert(IsValid());

    return (*m_pParent)->ResetBuffer(*m_handle);
}

Buffer::Buffer(std::shared_ptr<Handle> handle
    , std::shared_ptr<internal::BufferCollection*> pParent
)
    : m_pParent(pParent)
    , m_handle(handle)
{

}

}
}
