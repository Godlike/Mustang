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

/** @brief  Proxy class for controlling listener */
class Listener
{
public:
    //! Listener orientation description
    struct Orientation
    {
        //! Vector identifying forward direction
        std::array<float, 3> at;

        //! Vector identifying up direction
        std::array<float, 3> up;
    };

    /** @brief  Creates empty listener object
     *
     *  Created empty object is invalid
     */
    Listener();

    //! Default copy constructor
    Listener(Listener const& other) = default;

    //! Default assignment operator
    Listener& operator=(Listener const& other) = default;

    //! Default move constructor
    Listener(Listener&& other) = default;

    //! Default assignment-move operator
    Listener& operator=(Listener&& other) = default;

    //! Default destructor
    ~Listener() = default;

    /** @brief  Checks if object is valid and can be used
     *
     *  To be valid object must have non-expired #m_parent
     *
     *  @return @c true if object is valid, @c false otherwise
     */
    bool IsValid() const;

    //! Returns gain value
    float GetGain() const;

    //! Sets gain value
    bool SetGain(float value);

    //! Returns position
    std::array<float, 3> GetPosition() const;

    //! Sets position
    bool SetPosition(std::array<float, 3> vec);

    //! Returns orientation
    Orientation GetOrientation() const;

    //! Sets orientation
    bool SetOrientation(Orientation orientation);

private:
    friend class internal::ListenerController;

    /** @brief  Creates listener object
     *
     *  @param  parent  parent controller
     */
    explicit Listener(std::weak_ptr<internal::ListenerController> parent);

    //! Parent controller
    std::weak_ptr<internal::ListenerController> m_parent;
};

}
}

#endif // TULPAR_AUDIO_LISTENER_HPP
