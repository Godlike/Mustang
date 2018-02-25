/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_LOGGERS_HPP
#define TULPAR_LOGGERS_HPP

#include <mule/LoggerConfigBase.hpp>
#include <mule/templates/Singleton.hpp>

namespace tulpar
{

/** @brief  Public interface to internal tulpar logging categories */
class Loggers
    : public mule::LoggerConfigBase
    , public mule::templates::Singleton<Loggers>
{
public:
    using Settings = mule::LoggerConfigBase::Settings;

    /** @brief  Returns a vector of internal logging category names
     *
     *  @note   vector indices correspond to logging category indices
     */
    std::vector<std::string> GetDefaultLoggerNames() const;

private:
    friend mule::templates::Singleton<Loggers>;

    Loggers();
    ~Loggers() = default;

    virtual void InitializeLogger(uint32_t index, Settings const& settings) override;
};

}


#endif // TULPAR_LOGGERS_HPP
