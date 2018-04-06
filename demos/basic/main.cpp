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

    success = audio.Initialize(tulparConfig);

    if (success)
    {
        mule::asset::Storage& storage = mule::asset::Storage::Instance();
        mule::asset::Handler handler = storage.Get(testFile);

        tulpar::audio::Buffer buffer = audio.SpawnBuffer();
        success = buffer.BindData(handler);
        std::cout << (success ? "bound buffer" : "error binding buffer" ) << std::endl;

        if (success)
        {
            using namespace std::chrono_literals;

            tulpar::audio::Source source = audio.SpawnSource();
            source.SetLooping(false);

            {
                source.SetStaticBuffer(buffer);
                success = source.Play();

                while (tulpar::audio::Source::State::Stopped != source.GetState())
                {
                    std::cout << "Time: " << source.GetPlaybackPosition().count() << "ns\t";
                    std::cout << "Progress: " << source.GetPlaybackProgress();
                    std::cout << std::endl;

                    std::this_thread::sleep_for(100ms);
                }
            }

            {
                source.ResetBuffer();
                source.SetPosition({{ 0.0, 0.0f, 0.0f }});

                source.QueueBuffers({{ buffer, buffer, buffer }});

                float pitch = 1.0f;
                source.SetPitch(pitch);

                float delta = 0.05f;
                float factor = -1;

                float x = -0.4f;

                source.Play();

                while (tulpar::audio::Source::State::Stopped != source.GetState())
                {
                    pitch += factor * delta;
                    x += delta * 1e-1;

                    if (pitch <= 0.0f)
                    {
                        pitch = 0.0f;
                        factor = -factor;
                    }
                    else if (pitch >= 1.0f)
                    {
                        pitch = 1.0f;
                        factor = -factor;
                    }

                    source.SetPitch(std::max(pitch, 0.0f));
                    source.SetPosition({{ x, 0.0f, 1.0f }});

                    std::cout << "Time: " << source.GetPlaybackPosition().count() << "ns\t";
                    std::cout << "Progress: " << source.GetPlaybackProgress();
                    std::cout << std::endl;

                    std::this_thread::sleep_for(20ms);
                }
            }

            {
                source.ResetBuffer();
                source.SetPosition({{ 0.0, 0.0f, 0.0f }});

                source.SetStaticBuffer(buffer);
                source.SetPlaybackPosition(50ms);
                success = source.Play();

                while (tulpar::audio::Source::State::Stopped != source.GetState())
                {
                    std::cout << "Time: " << source.GetPlaybackPosition().count() << "ns\t";
                    std::cout << "Progress: " << source.GetPlaybackProgress();
                    std::cout << std::endl;

                    std::this_thread::sleep_for(100ms);
                }
            }

            {
                source.ResetBuffer();
                source.SetPosition({{ 0.0, 0.0f, 0.0f }});

                source.SetStaticBuffer(buffer);
                source.SetPlaybackProgress(0.2f);
                success = source.Play();

                while (tulpar::audio::Source::State::Stopped != source.GetState())
                {
                    std::cout << "Time: " << source.GetPlaybackPosition().count() << "ns\t";
                    std::cout << "Progress: " << source.GetPlaybackProgress();
                    std::cout << std::endl;

                    std::this_thread::sleep_for(100ms);
                }
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
