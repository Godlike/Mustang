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
    : m_pParent(nullptr)
    , m_handle(std::make_shared<Handle>(0))
{

}

bool Source::IsValid() const
{
    return (nullptr != m_handle.get())
        && (nullptr != m_pParent.get())
        && (*m_pParent)->IsValid(*m_handle);
}

Buffer Source::GetBuffer() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourceActiveBuffer(*m_handle);
}

void Source::ResetBuffer()
{
    SetStaticBuffer(Buffer());
}

Buffer Source::GetStaticBuffer() const
{
    assert(IsValid());
    assert(Type::Static == GetType());

    return (*m_pParent)->GetSourceStaticBuffer(*m_handle);
}

bool Source::SetStaticBuffer(Buffer buffer)
{
    assert(IsValid());
    assert(
        0 == buffer.GetSharedHandle() || (
            (State::Initial == GetState()) ||
            (State::Stopped == GetState())
        )
    );

    return (*m_pParent)->SetSourceStaticBuffer(*m_handle, (*buffer.GetSharedHandle()));
}

std::vector<Buffer> Source::GetQueuedBuffers() const
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return (*m_pParent)->GetSourceQueuedBuffers(*m_handle);
}

uint32_t Source::GetQueueIndex() const
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return (*m_pParent)->GetSourceQueueIndex(*m_handle);
}

bool Source::QueueBuffers(std::vector<Buffer> const& buffers)
{
    assert(IsValid());
    assert((Type::Undetermined == GetType()) || (Type::Streaming == GetType()));

    return (*m_pParent)->QueueSourceBuffers(*m_handle, buffers);
}

void Source::Reset()
{
    assert(IsValid());

    (*m_pParent)->ResetSource(*m_handle);
}

bool Source::Play()
{
    assert(IsValid());

    return (*m_pParent)->PlaySource(*m_handle);
}

bool Source::Stop()
{
    assert(IsValid());

    return (*m_pParent)->StopSource(*m_handle);
}

bool Source::Rewind()
{
    assert(IsValid());

    return (*m_pParent)->RewindSource(*m_handle);
}

bool Source::Pause()
{
    assert(IsValid());

    return (*m_pParent)->PauseSource(*m_handle);
}

std::chrono::nanoseconds Source::GetPlaybackDuration() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourcePlaybackDuration(*m_handle);
}

std::chrono::nanoseconds Source::GetPlaybackPosition() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourcePlaybackPosition(*m_handle);
}

bool Source::SetPlaybackPosition(std::chrono::nanoseconds offset)
{
    assert(IsValid());

    return (*m_pParent)->SetSourcePlaybackPosition(*m_handle, offset);
}

float Source::GetPlaybackProgress() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourcePlaybackProgress(*m_handle);
}

bool Source::SetPlaybackProgress(float value)
{
    assert(IsValid());

    return (*m_pParent)->SetSourcePlaybackProgress(*m_handle, value);
}

Source::State Source::GetState() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourceState(*m_handle);
}

Source::Type Source::GetType() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourceType(*m_handle);
}

bool Source::IsRelative() const
{
    assert(IsValid());

    return (*m_pParent)->IsSourceRelative(*m_handle);
}

bool Source::SetRelative(bool flag)
{
    assert(IsValid());

    return (*m_pParent)->SetSourceRelative(*m_handle, flag);
}

bool Source::IsLooping() const
{
    assert(IsValid());

    return (*m_pParent)->IsSourceLooping(*m_handle);
}

bool Source::SetLooping(bool flag)
{
    assert(IsValid());

    return (*m_pParent)->SetSourceLooping(*m_handle, flag);
}

float Source::GetPitch() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourcePitch(*m_handle);
}

bool Source::SetPitch(float value)
{
    assert(IsValid());

    return (*m_pParent)->SetSourcePitch(*m_handle, value);
}

float Source::GetGain() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourceGain(*m_handle);
}

bool Source::SetGain(float value)
{
    assert(IsValid());

    return (*m_pParent)->SetSourceGain(*m_handle, value);
}

std::array<float, 3> Source::GetPosition() const
{
    assert(IsValid());

    return (*m_pParent)->GetSourcePosition(*m_handle);
}

bool Source::SetPosition(std::array<float, 3> vec)
{
    assert(IsValid());

    return (*m_pParent)->SetSourcePosition(*m_handle, vec);
}

Source::Source(std::shared_ptr<Handle> handle
    , std::shared_ptr<internal::SourceCollection*> pParent
)
    : m_pParent(pParent)
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
