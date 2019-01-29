/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/audio/Listener.hpp>

#include <tulpar/internal/ListenerController.hpp>

#include <cassert>

namespace tulpar
{
namespace audio
{

Listener::Listener()
    : m_parent()
{

}

bool Listener::IsValid() const
{
    return !m_parent.expired();
}

float Listener::GetGain() const
{
    assert(IsValid());

    return m_parent.lock()->GetListenerGain();
}

bool Listener::SetGain(float value)
{
    assert(IsValid());

    return m_parent.lock()->SetListenerGain(value);
}

std::array<float, 3> Listener::GetPosition() const
{
    assert(IsValid());

    return m_parent.lock()->GetListenerPosition();
}

bool Listener::SetPosition(std::array<float, 3> vec)
{
    assert(IsValid());

    return m_parent.lock()->SetListenerPosition(vec);
}

Listener::Orientation Listener::GetOrientation() const
{
    assert(IsValid());

    return m_parent.lock()->GetListenerOrientation();
}

bool Listener::SetOrientation(Listener::Orientation orientation)
{
    assert(IsValid());

    return m_parent.lock()->SetListenerOrientation(orientation);
}

Listener::Listener(std::weak_ptr<internal::ListenerController> parent)
    : m_parent(parent)
{

}

}
}
