/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_DEMO_WORKING_DIR
#define TULPAR_DEMO_WORKING_DIR
#endif

#include <tulpar/TulparAudio.hpp>
#include <tulpar/TulparConfigurator.hpp>

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Source.hpp>

#include <tulpar/Loggers.hpp>

#include <mule/MuleUtilities.hpp>

#include <mule/asset/Handler.hpp>
#include <mule/asset/Storage.hpp>

#include <mule/Loggers.hpp>

#include <spdlog/sinks/ansicolor_sink.h>

#include <chrono>
#include <iostream>
#include <thread>

static std::string const testFile(TULPAR_DEMO_WORKING_DIR"data/ding_02.ogg");

int main()
{
    auto ansiSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    ansiSink->set_level(mule::LogLevel::trace);

    {
        mule::Loggers::Instance().SetDefaultSettings(
            mule::Loggers::Settings{
                std::string()
                , std::string("%+")
                , mule::LogLevel::info
                , { ansiSink }
            }
        );
    }

    mule::MuleUtilities::Initialize();

    {
        tulpar::Loggers::Instance().SetDefaultSettings(
            mule::Loggers::Settings{
                std::string()
                , std::string("%+")
                , mule::LogLevel::trace
                , { ansiSink }
            }
        );

        tulpar::Loggers::Instance().Reinitialize();
    }

    auto devices = tulpar::TulparConfigurator::GetDevices();

    tulpar::TulparConfigurator tulparConfig;
    tulparConfig.device = devices.front();

    tulpar::TulparAudio audio;

    bool success = true;

    success == audio.Initialize(tulparConfig);

    if (success)
    {
        mule::asset::Storage& storage = mule::asset::Storage::Instance();
        mule::asset::Handler handler = storage.Get(testFile);

        tulpar::audio::Buffer buffer = audio.SpawnBuffer();
        success = buffer.BindData(handler);
        std::cout << (success ? "bound buffer" : "error binding buffer" ) << std::endl;

        if (success)
        {
            tulpar::audio::Source source = audio.SpawnSource();
            source.BindBuffer(buffer);

            success = source.Play();

            std::cout << (success ? "playing" : "failed to play") << std::endl;

            if (success)
            {
                using namespace std::chrono_literals;

                std::this_thread::sleep_for(1s);
            }
        }

        audio.Deinitialize();
    }
    else
    {
        std::cerr << "Couldn't initialize Tulpar" << std::endl;
    }

    return 0;
}
