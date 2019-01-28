/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_TESTS_INTERNAL_COLLECTION_TEST_UTILS_HPP
#define TULPAR_TESTS_INTERNAL_COLLECTION_TEST_UTILS_HPP

#include <tulpar/internal/Collection.hpp>

#include <cstdint>
#include <memory>

namespace tulpar
{
namespace tests
{
namespace internal
{

static uint32_t s_incrementIndex = 0;

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

template<typename T>
    void DummyDeleter(typename tulpar::internal::Collection<T>::Handles const& handles)
{

}

}
}
}

#endif // TULPAR_TESTS_INTERNAL_COLLECTION_TEST_UTILS_HPP
