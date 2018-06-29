/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_DEVICE_HPP
#define TULPAR_INTERNAL_DEVICE_HPP

#include <AL/alc.h>

#include <tulpar/TulparConfigurator.hpp>

namespace tulpar
{
namespace internal
{

/** @brief  Audio output device
 *
 *  OpenAL device hosts audio::Buffer objects that were created while device
 *  was active (via associated Context::MakeCurrent() call)
 */
class Device
{
public:
    /** @brief  Create audio output device
     *
     *  Creates and initializes Device object using given @p config
     *
     *  @param  config  device configuration information
     *
     *  @return pointer to newly created Device when successful, @c nullptr otherwise
     *
     *  @sa Initialize
     */
    static Device* Create(TulparConfigurator::Device const& config);

    /** @brief  Destructs audio output device object
     *
     *  @sa Deinitialize
     */
    ~Device();

    //! Returns pointer to OpenAL Device object associated with this Device
    ALCdevice* GetOpenALDevice() const { return m_pDevice; }

private:
    //! Constructs empty audio output device object
    Device();

    /** @brief  Initializes audio output device object
     *
     *  Uses given name to open OpenAL device
     *
     *  @param  name    device name
     *
     *  @return @c true if device was initialized successfully, @c false otherwise
     */
    bool Initialize(std::string const& name);

    /** @brief  Deinitializes audio output device object
     *
     *  Closes associated OpenAL device
     */
    void Deinitialize();

    //! Flag indicating if object was initialized successfully
    bool m_isInitialized;

    //! Pointer to associated OpenAL device
    ALCdevice* m_pDevice;
};

}
}

#endif // TULPAR_INTERNAL_DEVICE_HPP
