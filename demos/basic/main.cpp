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
#include <tulpar/audio/Listener.hpp>
#include <tulpar/audio/Source.hpp>

#include <tulpar/Loggers.hpp>

#include <mule/MuleUtilities.hpp>

#include <mule/asset/Handler.hpp>
#include <mule/asset/Storage.hpp>

#include <mule/Loggers.hpp>

#include <spdlog/sinks/ansicolor_sink.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

static std::string const testFile(TULPAR_DEMO_WORKING_DIR"data/ding_02.ogg");

constexpr double g_pi = 3.14159265359;
constexpr double g_tau = g_pi * 2;

std::array<float, 3> GetCircleCoords(float radius, float progress)
{
    float const radians = progress * g_tau;

    return {{
        radius * std::sin(radians)      // x
        , 0.0f                          // y
        , -radius * std::cos(radians)   // z
    }};
}

std::array<float, 3> GetSpiralCoords(float radius, float progress)
{
    std::array<float, 3> coords = GetCircleCoords(radius, progress);

    coords[1] = radius * (progress - 0.5f) * 2.0f;  // y

    return coords;
}

void print(tulpar::audio::Source const& source)
{
    std::cout << "State: " << source.GetState() << "\t";
    std::cout << "Time: " << source.GetPlaybackPosition().count() << "ns\t";
    std::cout << "Progress: " << source.GetPlaybackProgress();
    std::cout << std::endl;
}

void SimplePlayback(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetLooping(false);
    source.SetStaticBuffer(buffer);

    source.Play();

    while (tulpar::audio::Source::State::Stopped != source.GetState())
    {
        print(source);

        std::this_thread::sleep_for(100ms);
    }

    source.Reset();
}

void PausePlayback(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetLooping(false);
    source.SetStaticBuffer(buffer);

    source.Play();

    while (tulpar::audio::Source::State::Stopped != source.GetState())
    {
        std::this_thread::sleep_for(100ms);

        print(source);

        switch (source.GetState())
        {
            case tulpar::audio::Source::State::Paused:
            {
                source.Play();
                break;
            }
            case tulpar::audio::Source::State::Playing:
            {
                source.Pause();
                break;
            }
            default:
            {
                break;
            }
        }
    }

    source.Reset();
}

void StopPlayback(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetLooping(false);
    source.SetStaticBuffer(buffer);

    source.Play();

    bool stop = false;

    while (!stop || tulpar::audio::Source::State::Stopped != source.GetState())
    {
        std::this_thread::sleep_for(100ms);

        print(source);

        if (!stop)
        {
            switch (source.GetState())
            {
                case tulpar::audio::Source::State::Stopped:
                {
                    stop = true;
                    source.Play();
                    break;
                }
                case tulpar::audio::Source::State::Playing:
                {
                    source.Stop();
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    source.Reset();
}

void MovingSource(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetLooping(false);
    source.QueueBuffers({{ buffer, buffer, buffer }});

    float pitch = 1.0f;
    source.SetPitch(pitch);

    float delta = 0.05f;
    float factor = -1;

    source.Play();

    float const radius = 1.5f;

    while (tulpar::audio::Source::State::Stopped != source.GetState())
    {
        pitch += factor * delta;

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
        source.SetPosition(GetSpiralCoords(radius, source.GetPlaybackProgress()));

        print(source);

        std::this_thread::sleep_for(20ms);
    }

    source.Reset();
}

void SeekPlayback(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetLooping(false);

    {
        source.SetStaticBuffer(buffer);
        source.SetPlaybackPosition(50ms);

        source.Play();

        while (tulpar::audio::Source::State::Stopped != source.GetState())
        {
            print(source);

            std::this_thread::sleep_for(100ms);
        }
    }

    {
        source.ResetBuffer();

        source.SetStaticBuffer(buffer);
        source.SetPlaybackProgress(0.2f);

        source.Play();

        while (tulpar::audio::Source::State::Stopped != source.GetState())
        {
            print(source);

            std::this_thread::sleep_for(100ms);
        }
    }

    {
        source.ResetBuffer();

        source.QueueBuffers({{ buffer, buffer, buffer, buffer }});
        source.SetPlaybackProgress(0.5f);

        source.Play();

        while (tulpar::audio::Source::State::Stopped != source.GetState())
        {
            print(source);

            std::this_thread::sleep_for(100ms);
        }
    }

    source.Reset();
}

void MovingListener(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Listener listener = audio.GetListener();

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetStaticBuffer(buffer);
    source.SetLooping(true);
    source.Play();

    {
        float const radius = 1.5f;

        std::chrono::milliseconds const duration = 4000ms;
        std::chrono::milliseconds const tick = 8ms;
        std::chrono::milliseconds offset = 0ms;

        while (offset < duration)
        {
            float const progress = static_cast<float>(offset.count()) / static_cast<float>(duration.count());

            listener.SetPosition(GetSpiralCoords(radius, progress));

            std::this_thread::sleep_for(tick);
            offset += tick;
        }
    }

    source.Stop();
    source.Reset();
}

void RotatingListener(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    tulpar::audio::Listener listener = audio.GetListener();

    tulpar::audio::Source source = audio.SpawnSource();

    source.SetStaticBuffer(buffer);
    source.SetPosition({{ 0.0f, 0.0f, 1.0f }});
    source.SetLooping(true);
    source.Play();

    {
        float const radius = 1.5f;

        std::chrono::milliseconds const duration = 4000ms;
        std::chrono::milliseconds const tick = 8ms;
        std::chrono::milliseconds offset = 0ms;

        while (offset < duration)
        {
            float const progress = static_cast<float>(offset.count()) / static_cast<float>(duration.count());

            listener.SetOrientation(tulpar::audio::Listener::Orientation{
                GetCircleCoords(radius, progress)
                , {{ 0.0f, 1.0f, 0.0f }}
            });

            std::this_thread::sleep_for(tick);
            offset += tick;
        }
    }

    source.Stop();
    source.Reset();
}

void SwitchingDevice(tulpar::TulparAudio& audio, tulpar::audio::Buffer& buffer)
{
    using namespace std::chrono_literals;

    auto devices = tulpar::TulparConfigurator::GetDevices();

    if (devices.size() > 1)
    {
        // static buffer
        {
            tulpar::TulparConfigurator secondDeviceConfig;
            secondDeviceConfig.device = devices[1];

            bool hasSwitched = false;

            tulpar::audio::Source source = audio.SpawnSource();
            source.SetLooping(false);

            source.SetStaticBuffer(buffer);
            source.Play();

            std::chrono::milliseconds sleepTime = 8ms;

            while (tulpar::audio::Source::State::Stopped != source.GetState())
            {
                print(source);

                if (!hasSwitched && source.GetPlaybackPosition() > 70ms)
                {
                    hasSwitched = true;
                    sleepTime = 100ms;

                    std::cout << "device switch started - " << secondDeviceConfig.device.name.c_str() << std::endl;

                    if (audio.Reinitialize(secondDeviceConfig))
                    {
                        std::cout << "device switch completed" << std::endl;
                    }
                    else
                    {
                        std::cout << "device switch failed" << std::endl;
                    }
                }

                std::this_thread::sleep_for(sleepTime);
            }

            source.Reset();
        }

        // buffer queue
        {
            tulpar::TulparConfigurator firstDeviceConfig;
            firstDeviceConfig.device = devices[0];

            bool hasSwitched = false;

            tulpar::audio::Source source = audio.SpawnSource();
            source.SetLooping(false);

            source.QueueBuffers({{ buffer, buffer }} );
            source.Play();

            std::chrono::milliseconds sleepTime = 8ms;

            while (tulpar::audio::Source::State::Stopped != source.GetState())
            {
                print(source);

                if (!hasSwitched && source.GetPlaybackPosition() > 150ms)
                {
                    hasSwitched = true;
                    sleepTime = 100ms;

                    std::cout << "device switch started - " << firstDeviceConfig.device.name.c_str() << std::endl;

                    if (audio.Reinitialize(firstDeviceConfig))
                    {
                        std::cout << "device switch completed" << std::endl;
                    }
                    else
                    {
                        std::cout << "device switch failed" << std::endl;
                    }
                }

                std::this_thread::sleep_for(sleepTime);
            }

            source.Reset();
        }
    }
    else
    {
        std::cerr << "SwitchingDevice() shall be run only if system has more than 1 audio device" << std::endl;
    }
}

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
            SimplePlayback(audio, buffer);
            PausePlayback(audio, buffer);
            StopPlayback(audio, buffer);
            MovingSource(audio, buffer);
            SeekPlayback(audio, buffer);
            MovingListener(audio, buffer);
            RotatingListener(audio, buffer);
            SwitchingDevice(audio, buffer);
        }

        audio.Deinitialize();
    }
    else
    {
        std::cerr << "Couldn't initialize Tulpar" << std::endl;
    }

    return 0;
}
