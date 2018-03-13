/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#define CATCH_CONFIG_MAIN

#include "CollectionTestUtils.hpp"

#include <tulpar/internal/BufferCollection.hpp>

#include <catch.hpp>

#include <cstdint>

namespace
{
static std::shared_ptr<tulpar::internal::BufferCollection> s_bufferCollection(nullptr);
}

void Setup()
{
    tulpar::tests::internal::s_incrementIndex = 0;
    s_bufferCollection.reset(new tulpar::internal::BufferCollection(
        tulpar::tests::internal::IncrementGenerator<tulpar::audio::Buffer>
        , tulpar::tests::internal::DummyReclaimer<tulpar::audio::Buffer>
        , tulpar::tests::internal::DummyDeleter<tulpar::audio::Buffer>
    ));
}

TEST_CASE("Buffer handle generation", "[collection]")
{
    using T = tulpar::audio::Buffer;

    Setup();

    GIVEN("collection with batch size of 1")
    {
        s_bufferCollection->Initialize(1);

        WHEN("items are created")
        {
            THEN("handles increment")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_bufferCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(i == object.GetHandle());
                }
            }
        }
        WHEN("items are reset after the creation")
        {
            THEN("handle stays the same")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_bufferCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(0 == object.GetHandle());

                    object.Reset();
                }
            }
        }
    }
}

TEST_CASE("Buffer collection resize", "[resize][collection]")
{
    using T = tulpar::audio::Buffer;

    Setup();

    GIVEN("collection with batch size of 2")
    {
        s_bufferCollection->Initialize(2);

        WHEN("buffer resizes and handles are reset")
        {
            T object0, object1, object2, object3;

            // create 0, 1, 2
            object0 = s_bufferCollection->Spawn();
            object1 = s_bufferCollection->Spawn();
            object2 = s_bufferCollection->Spawn();

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
                T object3 = s_bufferCollection->Spawn();
                REQUIRE(3 == object3.GetHandle());

                REQUIRE(true == object3.IsValid());

                // create 0, 2, 1
                object0 = s_bufferCollection->Spawn();
                REQUIRE(0 == object0.GetHandle());

                object2 = s_bufferCollection->Spawn();
                REQUIRE(2 == object2.GetHandle());

                object1 = s_bufferCollection->Spawn();
                REQUIRE(1 == object1.GetHandle());

                REQUIRE(true == object0.IsValid());
                REQUIRE(true == object1.IsValid());
                REQUIRE(true == object2.IsValid());
            }
        }
    }
}

TEST_CASE("Buffer handling", "[buffer]")
{
    using T = tulpar::audio::Buffer;

    Setup();

    GIVEN("collection with batch size of 1")
    {
        s_bufferCollection->Initialize(1);

        WHEN("items are created")
        {
            T object0 = s_bufferCollection->Spawn();
            T object1 = s_bufferCollection->Spawn();

            THEN("they are valid and have different handles")
            {
                REQUIRE(true == object0.IsValid());
                REQUIRE(true == object1.IsValid());

                REQUIRE(object0.GetHandle() != object1.GetHandle());
            }
        }
        WHEN("items are copied")
        {
            T objectA = s_bufferCollection->Spawn();
            T objectB(objectA);
            T objectC;
            objectC = objectA;

            THEN("they have the same handle and validness")
            {
                REQUIRE(objectA.GetHandle() == objectB.GetHandle());
                REQUIRE(objectB.GetHandle() == objectC.GetHandle());

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
                T object = s_bufferCollection->Spawn();

                REQUIRE(true == object.IsValid());

                object.Reset();

                REQUIRE(false == object.IsValid());
            }
        }
    }
}
