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

/** @brief  Tulpar configuration object */
class TulparConfigurator
{
public:
    /** @brief  Device description */
    struct Device
    {
        //! Device name
        std::string name;

        //! Flag indicating if device is default
        bool isDefault;
    };

    //! Creates configuration object
    TulparConfigurator();

    //! Default copy constructor
    TulparConfigurator(TulparConfigurator const& other) = default;

    //! Default assignment operator
    TulparConfigurator& operator=(TulparConfigurator const& other) = default;

    //! Default destructor
    ~TulparConfigurator() = default;

    //! Returns collection of available audio output devices
    static std::vector<Device> GetDevices();

    //! Buffer generation batch size
    uint32_t bufferBatch;

    //! Source generation batch size
    uint32_t sourceBatch;

    //! Device to be used
    Device device;
};

//! String representation ostream overload for configuraion object
std::ostream& operator<<(std::ostream& os, TulparConfigurator const& config);

}

#endif // TULPAR_TULPAR_CONFIGURATOR_HPP
