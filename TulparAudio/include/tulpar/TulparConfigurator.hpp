/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_TULPAR_CONFIGURATOR_HPP
#define TULPAR_TULPAR_CONFIGURATOR_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace tulpar
{

class TulparConfigurator
{
public:
    struct Device
    {
        std::string name;
        bool isDefault;
    };

    TulparConfigurator();

    TulparConfigurator(TulparConfigurator const& other) = default;
    TulparConfigurator& operator=(TulparConfigurator const& other) = default;

    ~TulparConfigurator() = default;

    static std::vector<Device> GetDevices();

    uint32_t bufferBatch;
    uint32_t sourceBatch;
    Device device;
};

std::ostream& operator<<(std::ostream& os, TulparConfigurator const& config);

}

#endif // TULPAR_TULPAR_CONFIGURATOR_HPP
