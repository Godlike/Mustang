/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_DEVICE_HPP
#define TULPAR_INTERNAL_DEVICE_HPP

#include <AL/alc.h>

#include <tulpar/TulparConfigurator.hpp>

namespace tulpar
{
namespace internal
{

class Device
{
public:
    static Device* Create(TulparConfigurator::Device const& config);
    ~Device();

    ALCdevice* GetOpenALDevice() const { return m_pDevice; }

private:
    Device();

    bool Initialize(std::string const& name);
    void Deinitialize();

    bool m_isInitialized;

    ALCdevice* m_pDevice;
};

}
}

#endif // TULPAR_INTERNAL_DEVICE_HPP
