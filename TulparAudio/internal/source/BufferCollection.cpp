/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/BufferCollection.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <AL/al.h>

#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#include <algorithm>

namespace tulpar
{
namespace internal
{

Collection<audio::Buffer>::Handles OpenAVBufferHandler::Generate(uint32_t batchSize)
{
    LOG_AUDIO->Trace("Generating {} buffers...", batchSize);

    Collection<audio::Buffer>::Handles result;
    result.reserve(batchSize);

    ALuint* alBuffers = new ALuint[batchSize];

    // clear error state
    ALenum alErr = alGetError();

    alGenBuffers(batchSize, alBuffers);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Generating {} buffers: {:#x}", batchSize, alErr);
    }

    std::transform(
        alBuffers
        , alBuffers + batchSize
        , std::back_inserter(result)
        , [](ALuint index) -> Collection<audio::Buffer>::Handle
        {
            return static_cast<Collection<audio::Buffer>::Handle>(index);
        }
    );

    delete[] alBuffers;

    return result;
}

void OpenAVBufferHandler::Reclaim(Collection<audio::Buffer>::Handle /*handle*/)
{

}

void OpenAVBufferHandler::Delete(Collection<audio::Buffer>::Handles const& handles)
{
    LOG_AUDIO->Trace("Deleting {} buffers...", handles.size());

    ALuint* alBuffers = new ALuint[handles.size()];

    std::transform(
        handles.begin()
        , handles.end()
        , alBuffers
        , [](Collection<audio::Buffer>::Handle handle) -> ALuint
        {
            return static_cast<ALuint>(handle);
        }
    );

    // clear error state
    ALenum alErr = alGetError();

    alDeleteBuffers(handles.size(), alBuffers);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Deleting {} buffers: {:#x}", handles.size(), alErr);
    }

    delete[] alBuffers;
}

BufferCollection::BufferCollection(
    Collection<audio::Buffer>::HandleGenerator generator
    , Collection<audio::Buffer>::HandleReclaimer reclaimer
    , Collection<audio::Buffer>::HandleDeleter deleter
)
    : Collection<audio::Buffer>(generator, reclaimer, deleter)
{

}

BufferCollection::~BufferCollection()
{

}

std::string BufferCollection::GetBufferName(Handle handle) const
{
    auto infoIt = m_bufferInfo.find(handle);

    return ((m_bufferInfo.cend() != infoIt) ? infoIt->second.name : std::string());
}

void BufferCollection::SetBufferName(Handle handle, std::string const& name)
{
    m_bufferInfo[handle].name = name;

    LOG_AUDIO->Debug("Buffer #{}: name = {}", handle, name.c_str());
}

uint8_t BufferCollection::GetBufferChannelCount(Handle handle) const
{
    auto infoIt = m_bufferInfo.find(handle);

    return ((m_bufferInfo.cend() != infoIt) ? infoIt->second.channels : 0);
}

uint32_t BufferCollection::GetBufferFrequencyHz(Handle handle) const
{
    auto infoIt = m_bufferInfo.find(handle);

    return ((m_bufferInfo.cend() != infoIt) ? infoIt->second.frequencyHz : 0);
}

uint32_t BufferCollection::GetBufferSampleCount(Handle handle) const
{
    auto infoIt = m_bufferInfo.find(handle);

    return ((m_bufferInfo.cend() != infoIt) ? infoIt->second.sampleCount : 0);
}

std::chrono::nanoseconds BufferCollection::GetBufferDuration(Handle handle) const
{
    auto infoIt = m_bufferInfo.find(handle);

    return ((m_bufferInfo.cend() != infoIt) ? infoIt->second.duration : std::chrono::nanoseconds(0));
}

bool BufferCollection::SetBufferData(Handle handle, mule::asset::Content const& content)
{
    ALuint index = static_cast<ALuint>(handle);

    LOG_AUDIO->Trace("Buffer #{}: parsing data with STB...", handle);

    int error = 0;
    stb_vorbis* pVorbis = stb_vorbis_open_memory(content.GetBuffer().data(), content.GetSize(), &error, NULL);

    if (VORBIS__no_error == error)
    {
        stb_vorbis_info vorbisInfo = stb_vorbis_get_info(pVorbis);
        uint32_t const sampleCount = stb_vorbis_stream_length_in_samples(pVorbis) * vorbisInfo.channels;

        ALshort* pSampleBuffer = new ALshort[sampleCount];

        stb_vorbis_get_samples_short_interleaved(pVorbis
            , vorbisInfo.channels
            , pSampleBuffer
            , sampleCount
        );

        LOG_AUDIO->Debug(
            "Buffer #{}: channels: {}; samples: {}; rate: {}"
            , handle
            , vorbisInfo.channels
            , sampleCount
            , vorbisInfo.sample_rate
        );

        // clear error state
        ALenum alErr = alGetError();

        alBufferData(index
            , ((1 == vorbisInfo.channels) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16)
            , pSampleBuffer
            , (sampleCount * sizeof(ALshort))
            , vorbisInfo.sample_rate
        );

        alErr = alGetError();

        if (AL_NO_ERROR == alErr)
        {
            BufferInfo& info = m_bufferInfo[handle];

            double const timeNs = 1e-9 * (static_cast<double>(info.sampleCount) / static_cast<double>(info.frequencyHz));

            info.channels = vorbisInfo.channels;
            info.frequencyHz = vorbisInfo.sample_rate;
            info.sampleCount = sampleCount;
            info.duration = std::chrono::nanoseconds(static_cast<uint64_t>(std::round(timeNs)));
        }
        else
        {
            LOG_AUDIO->Warning("Buffer #{}: binding data to OpenAL: {:#x}", handle, alErr);
        }

        delete[] pSampleBuffer;
        stb_vorbis_close(pVorbis);

        return (AL_NO_ERROR == alErr);
    }
    else // if open_memory indicated some error, we don't have to close any resources
    {
        LOG_AUDIO->Error("Buffer #{}: couldn't parse data", handle);

        return false;
    }
}

bool BufferCollection::ResetBuffer(Handle handle)
{
    LOG_AUDIO->Trace("Buffer #{}: reset", handle);

    Reclaim(handle);

    m_bufferInfo.erase(handle);

    return true;
}

audio::Buffer* BufferCollection::GenerateObject(Handle handle) const
{
    return new audio::Buffer(handle, const_cast<BufferCollection*>(this)->shared_from_this());
}

audio::Buffer* BufferCollection::CreateObject(Handle handle)
{
    SetBufferName(handle, std::string());

    return GenerateObject(handle);
}

}
}
