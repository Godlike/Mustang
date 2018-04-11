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

/** @brief  Base collection template
 *
 *  @tparam T   object type that has nested Handle type
 */
template<typename T>
    class Collection
{
public:
    //! Shortcut to object's handle type
    using Handle = typename T::Handle;

    //! Shortcut to a collection of handles
    using Handles = std::vector<Handle>;

    /** @brief  Shortcut to generator functor
     *
     *  Method shall generate a collection of handles of given size
     *
     *  @param  batchSize   number of handles to be created
     *
     *  @return collection of newly created handles
     */
    using HandleGenerator = Handles (*)(uint32_t batchSize);

    /** @brief  Shortcut to reclaimer functor
     *
     *  Method shall reset/invalidate given handle so that it could be reused
     *  in the future
     *
     *  @param  handle  handle to be reclaimed
     */
    using HandleReclaimer = void (*)(Handle handle);

    /** @brief  Shortcut to deleter functor
     *
     *  Method shall deinitialize given handles and they will not be reused
     *  unless they are generated again via @p m_generator
     *
     *  @param  handles collection of handles to be deleted
     */
    using HandleDeleter = void (*)(Handles const& handles);

    /** @brief  Constructs a collectiong using provided functors
     *
     *  @param  generator   functor that will be called when generating
     *                      new handle
     *  @param  reclaimer   functor that will be called when reclaiming
     *                      previously used handle
     *  @param  deleter     functor that will be called when releasing
     *                      previously used handle
     */
    Collection(HandleGenerator generator
        , HandleReclaimer reclaimer
        , HandleDeleter deleter
    );

    Collection(Collection const& other) = delete;
    Collection& operator=(Collection const& other) = delete;

    /** @brief  Destructs collection
     *
     *  Calls m_deleter for @p m_used to deinitialize all used handles
     */
    virtual ~Collection();

    /** @brief  Initializes collection settings
     *
     *  If size of @p m_available is smaller than @p batchSize, calls
     *  @p m_generator and PushHandles()
     *
     *  @param  batchSize   increment buffer size
     */
    void Initialize(uint32_t batchSize);

    /** @brief  Returns an object associated with given handle
     *
     *  Creates a copy of an object stored in @p m_objects associated with
     *  given @p handle
     *
     *  @param  handle  valid and used handle
     *
     *  @return a copy of an object associated with @p handle
     */
    T Get(Handle handle) const;

    /** @brief  Checks if given @p handled is registered within Collection
     *
     *  @param  handle  handle to check
     *
     *  @return @c true if handle is valid, @c false otherwise
     */
    bool IsValid(Handle handle) const;

    /** @brief  Spawns new object
     *
     *  Takes a handle from @p m_available
     *  Generates a batch of handles if needed
     *
     *  @return object associated with newly used handle
     */
    T Spawn();

protected:
    /** @brief  Reclaims given handle
     *
     *  Calls @p m_reclaimer
     *  Removes the handle from @p m_used and returns it to @p m_available
     *
     *  @param  handle  handle to be reclaimed
     */
    void Reclaim(Handle handle);

    /** @brief  Generate and initialize a batch of handles
     *
     *  Generates handle batches if needed
     *  Calls CreateObject() for each generated handle and stores the object
     *  in @p m_objects
     *
     *  @param  size    number of handles to generate
     *
     *  @return a collection of generated handles
     */
    Handles PrepareBatch(uint32_t size);

    /** @brief  Initializes an object for given handle
     *
     *  Initializes an object for given @p handle and stores it in @p m_objects
     *
     *  @return unique_ptr holding generated object
     */
    virtual std::unique_ptr<T> CreateObject(Handle handle) = 0;

    //! A batch size used when calling @p m_generator
    uint32_t m_batchSize;

    //! A functor used to generate a collection of handles of given size
    HandleGenerator m_generator;

    //! A functor used to reset/invalidate given handle
    HandleReclaimer m_reclaimer;

    //! A functor used to deinitialize given handles
    HandleDeleter m_deleter;

    //! An underlying collection of objects associated with handles
    std::unordered_map<Handle, std::unique_ptr<T>> m_objects;

    //! A collection of used handles
    Handles m_used;

    //! A queue of generated and unused handles
    std::queue<Handle> m_available;

private:
    //! Pushes provided handles to @p m_available
    void PushHandles(Handles const& handles);
};

}
}

#include <tulpar/internal/Collection.imp>

#endif // TULPAR_INTERNAL_COLLECTION_HPP
