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

class ListenerController
    : public std::enable_shared_from_this<ListenerController>
{
public:
    ListenerController() = default;

    ListenerController(ListenerController const& other) = delete;
    ListenerController& operator=(ListenerController const& other) = delete;

    ~ListenerController() = default;

    audio::Listener Get() const;

    float GetListenerGain() const;
    bool SetListenerGain(float value);

    std::array<float, 3> GetListenerPosition() const;
    bool SetListenerPosition(std::array<float, 3> const& vec);

    audio::Listener::Orientation GetListenerOrientation() const;
    bool SetListenerOrientation(audio::Listener::Orientation const& orientation);
};

}
}

#endif // TULPAR_INTERNAL_LISTENER_COLLECTION_HPP
