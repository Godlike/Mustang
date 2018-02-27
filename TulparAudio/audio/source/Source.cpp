/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/audio/Source.hpp>

#include <tulpar/internal/Collection.hpp>

namespace tulpar
{
namespace audio
{

bool Source::IsValid() const
{
    return !m_parent.expired() && m_parent.lock()->IsValid(m_handle);
}

void Source::BindBuffer(Buffer buffer)
{
    assert(IsValid());

    m_parent.lock()->BindBuffer(m_handle, buffer);
}

Buffer Source::GetBuffer() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourceBuffer(m_handle);
}

bool Source::Reset()
{
    assert(IsValid());

    return m_parent.lock()->ResetSource(m_handle);
}

Source::Source(Handle handle, std::weak_ptr<internal::SourceCollection> parent)
    : m_parent(parent)
    , m_handle(handle)
{

}

}
}
