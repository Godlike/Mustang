/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_TULPAR_AUDIO_HPP
#define TULPAR_TULPAR_AUDIO_HPP

#include <tulpar/TulparConfigurator.hpp>

#include <tulpar/audio/Buffer.hpp>
#include <tulpar/audio/Listener.hpp>
#include <tulpar/audio/Source.hpp>

#include <mule/asset/Handler.hpp>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace tulpar
{

namespace internal
{
class BufferCollection;
class Context;
class Device;
class ListenerController;
class SourceCollection;
}

/** @brief  Tulpar library entry point */
class TulparAudio
{
public:
    /** @brief Creates library instance
     *
     *  Initializes #m_listener
     */
    TulparAudio();

    //! Disable copy constructor
    TulparAudio(TulparAudio const& other) = delete;

    //! Disable assignment operator
    TulparAudio& operator=(TulparAudio const& other) = delete;

    //! Deinitializes library instance
    ~TulparAudio();

    /** @brief  Initialize library instance
     *
     *  Using provided @p config initializes #m_device, #m_context and collections
     *
     *  @param  config  tulpar configuration
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    bool Initialize(TulparConfigurator const& config);

    /** @brief  Reinitializes library instance
     *
     *  Using provided @p config initializes new device, context and collections
     *  If device changed, migrates all information stored in collections
     *
     *  @param  config  tulpar configuration
     *
     *  @return @c true if reinitialization was successful, @c false otherwise
     */
    bool Reinitialize(TulparConfigurator const& config);

    /** @brief Deinitializes library instance
     *
     *  Resets collections, context and device
     */
    void Deinitialize();

    //! Returns listener controller object
    audio::Listener GetListener() const;

    //! Returns source controller object identified by @p handle
    audio::Source GetSource(audio::Source::Handle handle) const;

    //! Spawns new source controller object
    audio::Source SpawnSource();

    //! Returns buffer controller object identified by @p handle
    audio::Buffer GetBuffer(audio::Buffer::Handle handle) const;

    //! Spawns new buffer controller object
    audio::Buffer SpawnBuffer();

private:
    //! Flag indicating if object was initialized successfully
    bool m_isInitialized;

    //! Device associated with library instance
    std::shared_ptr<internal::Device> m_device;

    //! Context associated with library instance
    std::shared_ptr<internal::Context> m_context;

    //! Listener controller
    std::shared_ptr<internal::ListenerController> m_listener;

    //! Audio buffer collection
    std::shared_ptr<internal::BufferCollection> m_buffers;

    //! Audio source collection
    std::shared_ptr<internal::SourceCollection> m_sources;
};

}

#endif // TULPAR_TULPAR_AUDIO_HPP
