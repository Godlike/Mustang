/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/audio/Source.hpp>

#include <tulpar/internal/SourceCollection.hpp>

namespace tulpar
{
namespace audio
{

Source::Source()
    : m_parent()
    , m_handle(0)
{

}

bool Source::IsValid() const
{
    return !m_parent.expired() && m_parent.lock()->IsValid(m_handle);
}

Buffer Source::GetBuffer() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourceActiveBuffer(m_handle);
}

void Source::ResetBuffer()
{
    SetStaticBuffer(Buffer());
}

Buffer Source::GetStaticBuffer() const
{
    assert(IsValid());
    assert(Type::Static == GetType());

    return m_parent.lock()->GetSourceStaticBuffer(m_handle);
}

bool Source::SetStaticBuffer(Buffer buffer)
{
    assert(IsValid());
    assert(
        0 == buffer.GetHandle() || (
            (State::Initial == GetState()) ||
            (State::Stopped == GetState())
        )
    );

    return m_parent.lock()->SetSourceStaticBuffer(m_handle, buffer);
}

std::vector<Buffer> Source::GetQueuedBuffers() const
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return m_parent.lock()->GetSourceQueuedBuffers(m_handle);
}

uint32_t Source::GetQueueIndex() const
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return m_parent.lock()->GetSourceQueueIndex(m_handle);
}

bool Source::QueueBuffers(std::vector<Buffer> const& buffers)
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return m_parent.lock()->QueueSourceBuffers(m_handle, buffers);
}

bool Source::Reset()
{
    assert(IsValid());

    return m_parent.lock()->ResetSource(m_handle);
}

bool Source::Play()
{
    assert(IsValid());

    return m_parent.lock()->PlaySource(m_handle);
}

bool Source::Stop()
{
    assert(IsValid());

    return m_parent.lock()->StopSource(m_handle);
}

bool Source::Rewind()
{
    assert(IsValid());

    return m_parent.lock()->RewindSource(m_handle);
}

bool Source::Pause()
{
    assert(IsValid());

    return m_parent.lock()->PauseSource(m_handle);
}

std::chrono::nanoseconds Source::GetPlaybackPosition() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourcePlaybackPosition(m_handle);
}

bool Source::SetPlaybackPosition(std::chrono::nanoseconds offset)
{
    assert(IsValid());

    return m_parent.lock()->SetSourcePlaybackPosition(m_handle, offset);
}

float Source::GetPlaybackProgress() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourcePlaybackProgress(m_handle);
}

bool Source::SetPlaybackProgress(float value)
{
    assert(IsValid());

    return m_parent.lock()->SetSourcePlaybackProgress(m_handle, value);
}

Source::State Source::GetState() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourceState(m_handle);
}

Source::Type Source::GetType() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourceType(m_handle);
}

bool Source::IsRelative() const
{
    assert(IsValid());

    return m_parent.lock()->IsSourceRelative(m_handle);
}

bool Source::SetRelative(bool flag)
{
    assert(IsValid());

    return m_parent.lock()->SetSourceRelative(m_handle, flag);
}

bool Source::IsLooping() const
{
    assert(IsValid());

    return m_parent.lock()->IsSourceLooping(m_handle);
}

bool Source::SetLooping(bool flag)
{
    assert(IsValid());

    return m_parent.lock()->SetSourceLooping(m_handle, flag);
}

float Source::GetPitch() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourcePitch(m_handle);
}

bool Source::SetPitch(float value)
{
    assert(IsValid());

    return m_parent.lock()->SetSourcePitch(m_handle, value);
}

float Source::GetGain() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourceGain(m_handle);
}

bool Source::SetGain(float value)
{
    assert(IsValid());

    return m_parent.lock()->SetSourceGain(m_handle, value);
}

std::array<float, 3> Source::GetPosition() const
{
    assert(IsValid());

    return m_parent.lock()->GetSourcePosition(m_handle);
}

bool Source::SetPosition(std::array<float, 3> vec)
{
    assert(IsValid());

    return m_parent.lock()->SetSourcePosition(m_handle, vec);
}

Source::Source(Handle handle, std::weak_ptr<internal::SourceCollection> parent)
    : m_parent(parent)
    , m_handle(handle)
{

}

std::ostream& operator<<(std::ostream& os, Source::State const& state)
{
    switch (state)
    {
        case Source::State::Unknown:
        {
            os << "Source::State::Unknown";
            break;
        }
        case Source::State::Initial:
        {
            os << "Source::State::Initial";
            break;
        }
        case Source::State::Playing:
        {
            os << "Source::State::Playing";
            break;
        }
        case Source::State::Paused:
        {
            os << "Source::State::Paused";
            break;
        }
        case Source::State::Stopped:
        {
            os << "Source::State::Stopped";
            break;
        }
        default:
        {
            os << "[unexpected Source::State]";
            break;
        }
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, Source::Type const& type)
{
    switch (type)
    {
        case Source::Type::Unknown:
        {
            os << "Source::Type::Unknown";
            break;
        }
        case Source::Type::Undetermined:
        {
            os << "Source::Type::Undetermined";
            break;
        }
        case Source::Type::Static:
        {
            os << "Source::Type::Static";
            break;
        }
        case Source::Type::Streaming:
        {
            os << "Source::Type::Streaming";
            break;
        }
        default:
        {
            os << "[unexpected Source::Type]";
            break;
        }
    }

    return os;
}

}
}
