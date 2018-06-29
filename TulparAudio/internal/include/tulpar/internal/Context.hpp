/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_CONTEXT_HPP
#define TULPAR_INTERNAL_CONTEXT_HPP

#include <tulpar/internal/Device.hpp>

#include <AL/alc.h>

namespace tulpar
{
namespace internal
{

/** @brief  Audio context
 *
 *  OpenAL context hosts audio::Source objects that were created while context
 *  was active (via Context::MakeCurrent() call)
 */
class Context
{
public:
    /** @brief  Create audio context
     *
     *  @param  device  device to be associated with context
     *
     *  @return pointer to newly created Context when successful, @c nullptr otherwise
     *
     *  @sa Initialize
     */
    static Context* Create(Device const& device);

    /** @brief  Destructs audio context object
     *
     *  @sa Deinitialize
     */
    ~Context();

    /** @brief  Make context object active */
    void MakeCurrent();

private:
    //! Constructs empty audio context object
    Context();

    /** @brief  Initializes audio context object
     *
     *  Creates audio context around given @p device
     *
     *  @param  device  device to be associated with context
     *
     *  @return @c true if context was initialized successfully, @c false otherwise
     */
    bool Initialize(Device const& device);

    /** @brief  Deinitializes audio context object
     *
     *  Destroys associated OpenAL context
     */
    void Deinitialize();

    //! Flag indicating if object was initialized successfully
    bool m_isInitialized;

    //! Pointer to associated OpenAL context
    ALCcontext* m_pContext;

    //! Pointer to associated OpenAL device
    ALCdevice* m_pDevice;
};

}
}

#endif // TULPAR_INTERNAL_CONTEXT_HPP
