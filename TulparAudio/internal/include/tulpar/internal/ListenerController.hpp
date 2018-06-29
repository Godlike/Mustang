/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_LISTENER_COLLECTION_HPP
#define TULPAR_INTERNAL_LISTENER_COLLECTION_HPP

#include <tulpar/audio/Listener.hpp>

#include <mule/asset/Content.hpp>

#include <array>
#include <memory>

namespace tulpar
{
namespace internal
{

/** @brief  Controller working with listener object */
class ListenerController
    : public std::enable_shared_from_this<ListenerController>
{
public:
    ListenerController() = default;

    ListenerController(ListenerController const& other) = delete;
    ListenerController& operator=(ListenerController const& other) = delete;

    ~ListenerController() = default;

    //! Creates listener object tied to this controller
    audio::Listener Get() const;

    //! Returns listener sound gain
    float GetListenerGain() const;

    //! Sets listener sound gain
    bool SetListenerGain(float value);

    //! Returns listener position in space
    std::array<float, 3> GetListenerPosition() const;

    //! Sets listener position in space
    bool SetListenerPosition(std::array<float, 3> const& vec);

    //! Returns listener orientation in space
    audio::Listener::Orientation GetListenerOrientation() const;

    //! Sets listener orientation in space
    bool SetListenerOrientation(audio::Listener::Orientation const& orientation);
};

}
}

#endif // TULPAR_INTERNAL_LISTENER_COLLECTION_HPP
