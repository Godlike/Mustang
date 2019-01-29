/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#define CATCH_CONFIG_MAIN

#include "CollectionTestUtils.hpp"

#include <tulpar/internal/BufferCollection.hpp>
#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/Loggers.hpp>

#include <mule/MuleUtilities.hpp>
#include <mule/Loggers.hpp>

#include <spdlog/sinks/ansicolor_sink.h>

#include <catch.hpp>

#include <cstdint>

namespace
{
static std::shared_ptr<tulpar::internal::SourceCollection> s_sourceCollection(nullptr);
static std::shared_ptr<tulpar::internal::BufferCollection> s_bufferCollection(nullptr);
}

void Setup()
{
    auto ansiSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    ansiSink->set_level(mule::LogLevel::trace);

    mule::Loggers::Instance().SetDefaultSettings(
        mule::Loggers::Settings{
            std::string()
            , std::string("%+")
            , mule::LogLevel::info
            , { ansiSink }
        }
    );

    mule::MuleUtilities::Initialize();

    tulpar::Loggers::Instance().SetDefaultSettings(
        mule::Loggers::Settings{
            std::string()
            , std::string("%+")
            , mule::LogLevel::trace
            , { ansiSink }
        }
    );

    tulpar::Loggers::Instance().Reinitialize();

    tulpar::tests::internal::s_incrementIndex = 0;

    s_bufferCollection.reset(new tulpar::internal::BufferCollection(
        tulpar::tests::internal::IncrementGenerator<tulpar::audio::Buffer>
        , tulpar::tests::internal::DummyReclaimer<tulpar::audio::Buffer>
        , tulpar::tests::internal::DummyDeleter<tulpar::audio::Buffer>
    ));

    s_sourceCollection.reset(new tulpar::internal::SourceCollection(
        *s_bufferCollection.get()
        , tulpar::tests::internal::IncrementGenerator<tulpar::audio::Source>
        , tulpar::tests::internal::DummyReclaimer<tulpar::audio::Source>
        , tulpar::tests::internal::DummyDeleter<tulpar::audio::Source>
    ));
}

TEST_CASE("Source handle generation", "[collection]")
{
    using T = tulpar::audio::Source;

    Setup();

    GIVEN("collection with batch size of 1")
    {
        s_sourceCollection->Initialize(1);

        WHEN("items are created")
        {
            THEN("handles increment")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_sourceCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(i == *(object.GetSharedHandle()));
                }
            }
        }
        WHEN("items are reset after the creation")
        {
            THEN("handle stays the same")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_sourceCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(0 == *(object.GetSharedHandle()));

                    object.Reset();
                }
            }
        }
    }
}

TEST_CASE("Source collection resize", "[resize][collection]")
{
    using T = tulpar::audio::Source;

    Setup();

    GIVEN("collection with batch size of 2")
    {
        s_sourceCollection->Initialize(2);

        WHEN("source resizes and handles are reset")
        {
            T object0, object1, object2, object3;

            // create 0, 1, 2
            object0 = s_sourceCollection->Spawn();
            object1 = s_sourceCollection->Spawn();
            object2 = s_sourceCollection->Spawn();

            // reset 0, 2, 1
            object0.Reset();
            object2.Reset();
            object1.Reset();

            REQUIRE(false == object0.IsValid());
            REQUIRE(false == object2.IsValid());
            REQUIRE(false == object1.IsValid());

            THEN("first handles from resize are used and then reset handles")
            {
                // create 3
                T object3 = s_sourceCollection->Spawn();
                REQUIRE(3 == *(object3.GetSharedHandle()));

                REQUIRE(true == object3.IsValid());

                // create 0, 2, 1
                object0 = s_sourceCollection->Spawn();
                REQUIRE(0 == *(object0.GetSharedHandle()));

                object2 = s_sourceCollection->Spawn();
                REQUIRE(2 == *(object2.GetSharedHandle()));

                object1 = s_sourceCollection->Spawn();
                REQUIRE(1 == *(object1.GetSharedHandle()));

                REQUIRE(true == object0.IsValid());
                REQUIRE(true == object1.IsValid());
                REQUIRE(true == object2.IsValid());
            }
        }
    }
}

TEST_CASE("Source handling", "[source]")
{
    using T = tulpar::audio::Source;

    Setup();

    GIVEN("collection with batch size of 1")
    {
        s_sourceCollection->Initialize(1);

        WHEN("items are created")
        {
            T object0 = s_sourceCollection->Spawn();
            T object1 = s_sourceCollection->Spawn();

            THEN("they are valid and have different handles")
            {
                REQUIRE(true == object0.IsValid());
                REQUIRE(true == object1.IsValid());

                REQUIRE(*(object0.GetSharedHandle()) != *(object1.GetSharedHandle()));
            }
        }
        WHEN("items are copied")
        {
            T objectA = s_sourceCollection->Spawn();
            T objectB(objectA);
            T objectC;
            objectC = objectA;

            THEN("they have the same handle and validness")
            {
                REQUIRE(*(objectA.GetSharedHandle()) == *(objectB.GetSharedHandle()));
                REQUIRE(*(objectB.GetSharedHandle()) == *(objectC.GetSharedHandle()));

                REQUIRE(true == objectA.IsValid());
                REQUIRE(true == objectB.IsValid());
                REQUIRE(true == objectC.IsValid());
            }
            THEN("they share common state after one of them is reset")
            {
                REQUIRE(true == objectA.IsValid());

                objectA.Reset();

                REQUIRE(false == objectA.IsValid());
                REQUIRE(false == objectB.IsValid());
                REQUIRE(false == objectC.IsValid());
            }
        }
        WHEN("items are reset")
        {
            THEN("they become invalid")
            {
                T object = s_sourceCollection->Spawn();

                REQUIRE(true == object.IsValid());

                object.Reset();

                REQUIRE(false == object.IsValid());
            }
        }
    }
}
