/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_DEVICE_HPP
#define TULPAR_INTERNAL_DEVICE_HPP

#include <AL/alc.h>

namespace tulpar
{
namespace internal
{

class Device
{
public:
    Device();
    ~Device();

    bool Initialize();
    void Deinitialize();

    ALCdevice* GetOpenALDevice() const { return m_pDevice; }

private:
    bool m_isInitialized;

    ALCdevice* m_pDevice;
};

}
}

#endif // TULPAR_INTERNAL_DEVICE_HPP
