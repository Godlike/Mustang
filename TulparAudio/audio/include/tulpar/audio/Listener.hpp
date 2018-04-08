/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_AUDIO_LISTENER_HPP
#define TULPAR_AUDIO_LISTENER_HPP

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

namespace tulpar
{

namespace internal
{
class ListenerController;
}

namespace audio
{

class Listener
{
public:
    struct Orientation
    {
        std::array<float, 3> at;
        std::array<float, 3> up;
    };

    Listener();

    Listener(Listener const& other) = default;
    Listener& operator=(Listener const& other) = default;

    Listener(Listener&& other) = default;
    Listener& operator=(Listener&& other) = default;

    ~Listener() = default;

    bool IsValid() const;

    float GetGain() const;
    bool SetGain(float value);

    std::array<float, 3> GetPosition() const;
    bool SetPosition(std::array<float, 3> vec);

    Orientation GetOrientation() const;
    bool SetOrientation(Orientation orientation);

private:
    friend class internal::ListenerController;

    explicit Listener(std::weak_ptr<internal::ListenerController> parent);

    std::weak_ptr<internal::ListenerController> m_parent;
};

}
}

#endif // TULPAR_AUDIO_LISTENER_HPP
