/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/TulparConfigurator.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <AL/alc.h>

namespace tulpar
{

TulparConfigurator::TulparConfigurator()
    : bufferBatch(32)
    , sourceBatch(32)
    , device(TulparConfigurator::Device
        {
            std::string()
            , true
        })
{

}

std::vector<TulparConfigurator::Device> TulparConfigurator::GetDevices()
{
    std::vector<TulparConfigurator::Device> result;

    std::string const defaultDevice(alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER));
    ALCchar const* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);

    if (nullptr != devices && '\0' != (*devices))
    {
        ALCchar const* it = devices;

        std::string name(it);
        it += name.size() + 1;

        result.push_back({name, (0 == defaultDevice.compare(name))});

        while (*it != '\0')
        {
            name = it;
            it += name.size() + 1;

            result.push_back({name, (0 == defaultDevice.compare(name))});
        }

        LOG->Debug("TulparConfigurator::GetDevices() found {} devices", result.size());
        for (auto const& device : result)
        {
            LOG->Debug("TulparConfigurator::GetDevices()\t{}{}", device.name.c_str(), device.isDefault ? " (default)" : "");
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, TulparConfigurator const& config)
{
    return os << "TulparConfigurator { "
        << "bufferBatch: " << config.bufferBatch
        << ", sourceBatch: " << config.sourceBatch
        << ", device: { "
        << " name: \"" << config.device.name.c_str() << "\""
        << ", default: " << (config.device.isDefault ? "true" : "false")
        << " } }";
}

}
