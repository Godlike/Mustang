/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_COLLECTION_HPP
#define TULPAR_INTERNAL_COLLECTION_HPP

#include <cstdint>
#include <queue>
#include <memory>
#include <unordered_map>
#include <vector>

namespace tulpar
{
namespace internal
{

template<typename T>
    class Collection
{
public:
    using Handle = typename T::Handle;
    using Handles = std::vector<Handle>;
    using HandleGenerator = Handles (*)(uint32_t batchSize);
    using HandleReclaimer = void (*)(Handle handle);
    using HandleDeleter = void (*)(Handles const& handles);

    Collection(HandleGenerator generator
        , HandleReclaimer reclaimer
        , HandleDeleter deleter
    );

    Collection(Collection const& other) = delete;
    Collection& operator=(Collection const& other) = delete;

    virtual ~Collection();

    void Initialize(uint32_t batchSize);

    T Get(Handle handle) const;
    bool IsValid(Handle handle) const;
    T Spawn();

protected:
    void Reclaim(Handle handle);
    Handles PrepareBatch(uint32_t size);

    virtual std::unique_ptr<T> GenerateObject(Handle handle) const = 0;
    virtual std::unique_ptr<T> CreateObject(Handle handle) = 0;

    uint32_t m_batchSize;

    HandleGenerator m_generator;
    HandleReclaimer m_reclaimer;
    HandleDeleter m_deleter;

    std::unordered_map<Handle, std::unique_ptr<T>> m_objects;

    Handles m_used;
    std::queue<Handle> m_available;

private:
    void PushHandles(Handles const& handles);
};

}
}

#include <tulpar/internal/Collection.imp>

#endif // TULPAR_INTERNAL_COLLECTION_HPP
