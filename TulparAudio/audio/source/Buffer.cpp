/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/audio/Buffer.hpp>

#include <tulpar/internal/Collection.hpp>

namespace tulpar
{
namespace audio
{

bool Buffer::IsValid() const
{
    return !m_parent.expired() && m_parent.lock()->IsValid(m_handle);
}

bool Buffer::BindData(mule::asset::Handler asset)
{
    assert(IsValid());

    auto parent = m_parent.lock();

    parent->SetBufferName(m_handle, asset.GetName());

    return parent->SetBufferData(m_handle, asset.GetContent());
}

std::string Buffer::GetDataName() const
{
    assert(IsValid());

    return m_parent.lock()->GetBufferName(m_handle);
}

bool Buffer::Reset()
{
    assert(IsValid());

    return m_parent.lock()->ResetBuffer(m_handle);
}

Buffer::Buffer(Handle handle, std::weak_ptr<internal::BufferCollection> parent)
    : m_parent(parent)
    , m_handle(handle)
{

}

}
}
