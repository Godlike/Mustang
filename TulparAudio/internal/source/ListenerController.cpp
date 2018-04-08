/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/ListenerController.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <AL/al.h>

#include <limits>

namespace tulpar
{
namespace internal
{

audio::Listener ListenerController::Get() const
{
    return audio::Listener(const_cast<ListenerController*>(this)->shared_from_this());
}

float ListenerController::GetListenerGain() const
{
    // clear error state
    ALenum alErr = alGetError();

    ALfloat result = 0.0f;

    alGetListenerf(AL_GAIN, &result);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        result = -1.0f;
        LOG_AUDIO->Warning("Listener: get gain: {:#x}", alErr);
    }

    return result;
}

bool ListenerController::SetListenerGain(float value)
{
    LOG_AUDIO->Debug("Listener: set gain {}", value);

    // clear error state
    ALenum alErr = alGetError();

    alListenerf(AL_GAIN, value);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Listener: set gain: {:#x}", alErr);
    }

    return AL_NO_ERROR == alErr;
}

std::array<float, 3> ListenerController::GetListenerPosition() const
{
    // clear error state
    ALenum alErr = alGetError();

    ALfloat x;
    ALfloat y;
    ALfloat z;

    alGetListener3f(AL_POSITION, &x, &y, &z);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Listener: get position: {:#x}", alErr);

        x = std::numeric_limits<float>::quiet_NaN();
        y = std::numeric_limits<float>::quiet_NaN();
        z = std::numeric_limits<float>::quiet_NaN();
    }

    return {{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }};
}

bool ListenerController::SetListenerPosition(std::array<float, 3> const& vec)
{
    LOG_AUDIO->Debug("Listener: set position {{ {}, {}, {} }}", vec[0], vec[1], vec[2]);

    // clear error state
    ALenum alErr = alGetError();

    alListener3f(AL_POSITION, static_cast<ALfloat>(vec[0]), static_cast<ALfloat>(vec[1]), static_cast<ALfloat>(vec[2]));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Listener: set position: {:#x}", alErr);
    }

    return AL_NO_ERROR == alErr;
}

audio::Listener::Orientation ListenerController::GetListenerOrientation() const
{
    audio::Listener::Orientation result;

    // clear error state
    ALenum alErr = alGetError();

    ALfloat values[6];

    alGetListenerfv(AL_ORIENTATION, values);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        result.at = {{ values[0], values[1], values[2] }};
        result.up = {{ values[3], values[4], values[5] }};
    }
    else
    {
        LOG_AUDIO->Warning("Listener: get position: {:#x}", alErr);

        result.at = {{ std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() }};
        result.up = {{ std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() }};
    }

    return result;
}

bool ListenerController::SetListenerOrientation(audio::Listener::Orientation const& orientation)
{
    LOG_AUDIO->Debug("Listener: set orientation {{ at: {{ {}, {}, {} }}, up: {{ {}, {}, {} }} }}"
        , orientation.at[0], orientation.at[1], orientation.at[2]
        , orientation.up[0], orientation.up[1], orientation.up[2]
    );

    ALfloat values[6];

    values[0] = orientation.at[0];
    values[1] = orientation.at[1];
    values[2] = orientation.at[2];

    values[3] = orientation.up[0];
    values[4] = orientation.up[1];
    values[5] = orientation.up[2];

    // clear error state
    ALenum alErr = alGetError();

    alListenerfv(AL_ORIENTATION, values);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Listener: set orientation: {:#x}", alErr);
    }

    return AL_NO_ERROR == alErr;
}

}
}
