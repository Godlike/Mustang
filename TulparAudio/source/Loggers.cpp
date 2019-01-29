/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/Loggers.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <algorithm>
#include <cassert>

namespace tulpar
{

std::array<mule::LoggerPtr, Log::size> g_loggers;

namespace
{

static std::array<char const*, Log::size> s_logger_names = {{
    "tulpar"
    , "tulpar_audio"
}};

}

std::vector<std::string> Loggers::GetDefaultLoggerNames() const
{
    std::vector<std::string> result;

    result.reserve(Log::size);

    std::transform(
        s_logger_names.begin(), s_logger_names.end()
        , std::back_inserter(result)
        , [](char const* name) -> std::string { return std::string(name); }
    );

    return result;
}

Loggers::Loggers()
    : mule::LoggerConfigBase(Log::size)
    , mule::templates::Singleton<Loggers>()
{

}

void Loggers::InitializeLogger(uint32_t index, Settings const& settings)
{
    assert(index < Log::size);

    g_loggers[index] = std::make_shared<mule::Logger>(
        (!settings.name.empty() ? settings.name : s_logger_names[index])
        , settings.sinks.begin()
        , settings.sinks.end()
    );

    g_loggers[index]->SetLevel(settings.level);
    g_loggers[index]->SetPattern(settings.pattern);
}

}
