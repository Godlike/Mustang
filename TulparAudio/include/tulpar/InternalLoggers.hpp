/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_LOGGERS_HPP
#define TULPAR_INTERNAL_LOGGERS_HPP

#include <mule/Logger.hpp>

#include <array>
#include <cstdint>

namespace tulpar
{

struct Log
{
    static constexpr uint32_t tulpar = 0;
    static constexpr uint32_t tulpar_audio = 1;

    static constexpr uint32_t size = 2;
};

extern std::array<mule::LoggerPtr, Log::size> g_loggers;

}

#define LOG ::tulpar::g_loggers[::tulpar::Log::tulpar]
#define LOG_AUDIO ::tulpar::g_loggers[::tulpar::Log::tulpar_audio]

#endif // TULPAR_INTERNAL_LOGGERS_HPP
