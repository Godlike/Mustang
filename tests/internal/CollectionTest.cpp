#define CATCH_CONFIG_MAIN

#include <tulpar/internal/BufferCollection.hpp>

#include <catch.hpp>

#include <memory>

namespace
{

static volatile uint32_t s_incrementIndex = 0;
static std::shared_ptr<tulpar::internal::BufferCollection> s_bufferCollection(nullptr);

template<typename T>
    typename tulpar::internal::Collection<T>::Handles IncrementGenerator(uint32_t batchSize)
{
    typename tulpar::internal::Collection<T>::Handles result;

    result.reserve(batchSize);

    for (uint32_t i = 0; i < batchSize; ++i)
    {
        result.push_back(s_incrementIndex++);
    }

    return result;
}

template<typename T>
    void DummyReclaimer(typename tulpar::internal::Collection<T>::Handle handle)
{

}

}

template<typename T>
    void Setup()
{
    s_incrementIndex = 0;
    s_bufferCollection.reset(new tulpar::internal::BufferCollection(IncrementGenerator<T>, DummyReclaimer<T>));
}

TEST_CASE("Buffer generation", "[basic]")
{
    using T = tulpar::audio::Buffer;

    Setup<T>();

    GIVEN("Collection with batch size of 1")
    {
        s_bufferCollection->Initialize(1);

        WHEN("Items are reset after the creation")
        {
            THEN("Handles stay the same")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_bufferCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(object.IsValid());
                    REQUIRE(0 == object.GetHandle());

                    object.Reset();
                }
            }
        }
        AND_WHEN("Items are created one after another")
        {
            THEN("Handles increment")
            {
                for (uint32_t i = 0; i < 4; ++i)
                {
                    T object = s_bufferCollection->Spawn();

                    CAPTURE(i);

                    REQUIRE(object.IsValid());
                    REQUIRE(i == object.GetHandle());
                }
            }
        }
    }
}
