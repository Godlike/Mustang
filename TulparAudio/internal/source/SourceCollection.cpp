/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <AL/al.h>

#include <algorithm>
#include <limits>

namespace tulpar
{
namespace internal
{

Collection<audio::Source>::Handles OpenAVSourceHandler::Generate(uint32_t batchSize)
{
    LOG_AUDIO->Trace("Generating {} sources...", batchSize);

    Collection<audio::Source>::Handles result;
    result.reserve(batchSize);

    ALuint* alSources = new ALuint[batchSize];

    // clear error state
    ALenum alErr = alGetError();

    alGenSources(batchSize, alSources);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Generating {} sources: {:#x}", batchSize, alErr);
    }

    std::transform(
        alSources
        , alSources + batchSize
        , std::back_inserter(result)
        , [](ALuint index) -> Collection<audio::Source>::Handle
        {
            alSourcef(index, AL_PITCH, 1);
            alSourcef(index, AL_GAIN, 1);
            alSource3f(index, AL_POSITION, 0, 0, 0);
            alSource3f(index, AL_VELOCITY, 0, 0, 0);
            alSourcei(index, AL_LOOPING, AL_FALSE);

            return static_cast<Collection<audio::Source>::Handle>(index);
        }
    );

    delete[] alSources;

    return result;
}

void OpenAVSourceHandler::Reclaim(Collection<audio::Source>::Handle handle)
{
    LOG_AUDIO->Trace("Source #{}: reclaim", handle);

    // clear error state
    ALenum alErr = alGetError();

    alSourceStop(static_cast<ALuint>(handle));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{} stop: {:#x}", handle, alErr);
    }
}

void OpenAVSourceHandler::Delete(Collection<audio::Source>::Handles const& handles)
{
    LOG_AUDIO->Trace("Deleting {} sources...", handles.size());

    ALuint* alSources = new ALuint[handles.size()];

    std::transform(
        handles.begin()
        , handles.end()
        , alSources
        , [](Collection<audio::Source>::Handle handle) -> ALuint
        {
            return static_cast<ALuint>(handle);
        }
    );

    // clear error state
    ALenum alErr = alGetError();

    alDeleteSources(handles.size(), alSources);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Deleting {} sources: {:#x}", handles.size(), alErr);
    }

    delete[] alSources;
}

SourceCollection::SourceCollection(BufferCollection const& buffers
    , Collection<audio::Source>::HandleGenerator generator
    , Collection<audio::Source>::HandleReclaimer reclaimer
    , Collection<audio::Source>::HandleDeleter deleter
)
    : Collection<audio::Source>(generator, reclaimer, deleter)
    , m_buffers(buffers)
{

}

namespace
{

struct MigrationInfo
{
    SourceCollection::BufferHandle staticBuffer;
    std::vector<SourceCollection::BufferHandle> queuedBuffers;

    audio::Source::State state;
    audio::Source::Type type;

    ALint sampleOffset;

    std::array<float, 3> position;

    float pitch;
    float gain;

    bool isRelative;
    bool isLooping;
};

}

void SourceCollection::InheritCollection(SourceCollection const& other
    , BufferCollection::MigrationMapping const& bufferMapping
    , Context& oldContext
    , Context& newContext
)
{
    assert(this != &other);

    std::vector<SourceHandle> const& old = other.m_used;

    if (!old.empty())
    {
        // gather old data from the old context
        oldContext.MakeCurrent();

        std::unordered_map<SourceHandle, MigrationInfo> info;
        ALuint* tmpSources = new ALuint[old.size()];

        // get old states
        {
            uint32_t i = 0;

            for (SourceHandle const& handle : old)
            {
                MigrationInfo& migrate = info[handle];

                migrate.state = other.GetSourceState(handle);
                tmpSources[i++] = static_cast<ALuint>(handle);
            }

            // pause all old sources
            alSourcePausev(i, tmpSources);
        }

        // get other information
        {
            for (SourceHandle const& handle : old)
            {
                MigrationInfo& migrate = info[handle];

                migrate.type = other.GetSourceType(handle);

                switch (migrate.type)
                {
                    case audio::Source::Type::Static:
                    {
                        migrate.staticBuffer = other.m_sourceBuffers.at(handle);
                        break;
                    }
                    case audio::Source::Type::Streaming:
                    {
                        migrate.queuedBuffers = other.m_sourceQueuedBuffers.at(handle);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                alGetSourcei(static_cast<ALuint>(handle), AL_SAMPLE_OFFSET, &migrate.sampleOffset);

                migrate.position = other.GetSourcePosition(handle);

                migrate.pitch = other.GetSourcePitch(handle);
                migrate.gain = other.GetSourceGain(handle);

                migrate.isRelative = other.IsSourceRelative(handle);
                migrate.isLooping = other.IsSourceLooping(handle);
            }
        }

        // restore old data in the new context
        newContext.MakeCurrent();

        {
            std::vector<SourceHandle> batch = PrepareBatch(old.size());
            uint32_t i = 0;
            uint32_t playingSources = 0;

            for (SourceHandle const& oldHandle : old)
            {
                MigrationInfo const& migrate = info.at(oldHandle);

                SourceHandle newHandle = batch[i++];

                {
                    audio::Source const& oldObject = other.m_objects.at(oldHandle);
                    *(oldObject.m_pParent) = this;
                    *(oldObject.m_handle) = newHandle;

                    audio::Source& newObject = m_objects.at(newHandle);
                    newObject = oldObject;
                }

                switch (migrate.state)
                {
                    case audio::Source::State::Playing:
                    {
                        tmpSources[playingSources++] = static_cast<ALuint>(newHandle);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                switch (migrate.type)
                {
                    case audio::Source::Type::Static:
                    {
                        assert(bufferMapping.cend() != bufferMapping.find(migrate.staticBuffer));

                        SetSourceStaticBuffer(newHandle, bufferMapping.at(migrate.staticBuffer));
                        break;
                    }
                    case audio::Source::Type::Streaming:
                    {
                        std::vector<audio::Buffer> buffers;
                        buffers.reserve(migrate.queuedBuffers.size());

                        std::transform(migrate.queuedBuffers.cbegin()
                            , migrate.queuedBuffers.cend()
                            , std::back_inserter(buffers)
                            , [&](BufferHandle const& oldBuffer) -> audio::Buffer
                            {
                                assert(bufferMapping.cend() != bufferMapping.find(oldBuffer));

                                return m_buffers.Get(bufferMapping.at(oldBuffer));
                            }
                        );

                        QueueSourceBuffers(newHandle, buffers);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                alSourcei(static_cast<ALuint>(newHandle), AL_SAMPLE_OFFSET, migrate.sampleOffset);

                SetSourcePosition(newHandle, migrate.position);

                SetSourcePitch(newHandle, migrate.pitch);
                SetSourceGain(newHandle, migrate.gain);

                SetSourceRelative(newHandle, migrate.isRelative);
                SetSourceLooping(newHandle, migrate.isLooping);
            }

            if (playingSources)
            {
                alSourcePlayv(playingSources, tmpSources);
            }
        }

        delete[] tmpSources;
    }
}

audio::Buffer SourceCollection::GetSourceActiveBuffer(SourceHandle source) const
{
    audio::Buffer buffer;

    switch (GetSourceType(source))
    {
        case audio::Source::Type::Static:
        {
            buffer = GetSourceStaticBuffer(source);

            break;
        }
        case audio::Source::Type::Streaming:
        {
            std::vector<audio::Buffer> queue = GetSourceQueuedBuffers(source);
            buffer = queue[GetSourceQueueIndex(source)];

            break;
        }
        default:
        {
            break;
        }
    }

    return buffer;
}

std::vector<audio::Buffer> SourceCollection::GetSourceActiveBuffers(SourceHandle source) const
{
    std::vector<audio::Buffer> queue;

    switch (GetSourceType(source))
    {
        case audio::Source::Type::Static:
        {
            queue.push_back(GetSourceStaticBuffer(source));

            break;
        }
        case audio::Source::Type::Streaming:
        {
            queue = GetSourceQueuedBuffers(source);

            break;
        }
        default:
        {
            break;
        }
    }

    return queue;
}

audio::Buffer SourceCollection::GetSourceStaticBuffer(SourceHandle source) const
{
    assert(m_sourceBuffers.cend() != m_sourceBuffers.find(source));

    return m_buffers.Get(m_sourceBuffers.at(source));
}

bool SourceCollection::SetSourceStaticBuffer(SourceHandle source, BufferHandle buffer)
{
    LOG_AUDIO->Debug("Source #{}: buffer = #{}", source, buffer);

    m_sourceBuffers[source] = buffer;

    // clear error state
    ALenum alErr = alGetError();

    alSourcei(static_cast<ALuint>(source)
        , AL_BUFFER
        , static_cast<ALuint>(buffer)
    );

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        using namespace std::chrono_literals;

        Meta& meta = m_sourceMeta[source];

        if (0 != buffer)
        {
            audio::Buffer buf = m_buffers.Get(buffer);

            meta.activeSampleCount = buf.GetSampleCount();
            meta.activeTotalDuration = buf.GetDuration();
        }
        else
        {
            meta.activeSampleCount = 0;
            meta.activeTotalDuration = 0ns;
        }

        m_sourceQueuedBuffers[source].clear();
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: buffer = #{}: {:#x}", source, buffer, alErr);
    }

    return AL_NO_ERROR == alErr;
}

std::vector<audio::Buffer> SourceCollection::GetSourceQueuedBuffers(SourceHandle source) const
{
    std::vector<audio::Buffer> result;

    // clear error state
    ALenum alErr = alGetError();

    ALint alQueueLength;
    alGetSourcei(static_cast<ALuint>(source), AL_BUFFERS_QUEUED, &alQueueLength);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        if (alQueueLength > 0)
        {
            std::vector<BufferHandle> const& queue = m_sourceQueuedBuffers.at(source);

            assert(static_cast<size_t>(alQueueLength) == queue.size());

            result.resize(alQueueLength);

            std::transform(
                queue.cbegin()
                , queue.cbegin() + static_cast<size_t>(alQueueLength)
                , result.begin()
                , [&](BufferHandle const& handle) -> audio::Buffer
                {
                    return m_buffers.Get(handle);
                }
            );
        }
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: get buffer queue: {:#x}", source, alErr);
    }

    return result;
}

uint32_t SourceCollection::GetSourceQueueIndex(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALint alQueueIndex;
    alGetSourcei(static_cast<ALuint>(source), AL_BUFFERS_PROCESSED, &alQueueIndex);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: get buffer queue index: {:#x}", source, alErr);
        alQueueIndex = 0;
    }

    return alQueueIndex;
}

bool SourceCollection::QueueSourceBuffers(SourceHandle source, std::vector<audio::Buffer> const& buffers)
{
    LOG_AUDIO->Debug("Source #{}: set buffer queue[{}]", source, buffers.size());

    std::vector<ALuint> tmp(buffers.size(), 0);

    std::transform(buffers.begin(), buffers.end(), tmp.begin(),
        [](audio::Buffer const& buffer) -> ALuint
        {
            return static_cast<ALuint>(*(buffer.GetSharedHandle()));
        }
    );

    // clear error state
    ALenum alErr = alGetError();

    alSourceQueueBuffers(static_cast<ALuint>(source), tmp.size(), tmp.data());

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        using namespace std::chrono_literals;

        std::vector<BufferHandle>& queue = m_sourceQueuedBuffers[source];
        queue.reserve(buffers.size());

        Meta& meta = m_sourceMeta[source];

        meta.activeSampleCount = 0;
        meta.activeTotalDuration = 0ns;

        for (audio::Buffer const& buffer : buffers)
        {
            meta.activeSampleCount += buffer.GetSampleCount();
            meta.activeTotalDuration += buffer.GetDuration();

            queue.push_back(*(buffer.GetSharedHandle()));
        }

        m_sourceBuffers[source] = *(audio::Buffer().GetSharedHandle());
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: set buffer queue[{}]: {:#x}", source, tmp.size(), alErr);
    }

    return AL_NO_ERROR == alErr;
}

void SourceCollection::ResetSource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: reset", source);

    Reclaim(source);
    SetSourceStaticBuffer(source, *(audio::Buffer().GetSharedHandle()));

    m_sourceMeta.erase(source);
    m_sourceBuffers.erase(source);
    m_sourceQueuedBuffers.erase(source);
}

bool SourceCollection::PlaySource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: play", source);

    // clear error state
    ALenum alErr = alGetError();

    alSourcePlay(static_cast<ALuint>(source));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: play: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

bool SourceCollection::StopSource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: stop", source);

    // clear error state
    ALenum alErr = alGetError();

    alSourceStop(static_cast<ALuint>(source));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: stop: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

bool SourceCollection::RewindSource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: rewind", source);

    // clear error state
    ALenum alErr = alGetError();

    alSourceRewind(static_cast<ALuint>(source));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: rewind: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

bool SourceCollection::PauseSource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: pause", source);

    // clear error state
    ALenum alErr = alGetError();

    alSourcePause(static_cast<ALuint>(source));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: pause: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

std::chrono::nanoseconds SourceCollection::GetSourcePlaybackDuration(SourceHandle source) const
{
    return m_sourceMeta.at(source).activeTotalDuration;
}

std::chrono::nanoseconds SourceCollection::GetSourcePlaybackPosition(SourceHandle source) const
{
    std::chrono::nanoseconds result(0);

    // clear error state
    ALenum alErr = alGetError();

    ALint sampleOffset;
    alGetSourcei(static_cast<ALuint>(source), AL_SAMPLE_OFFSET, &sampleOffset);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        audio::Source::State const state = GetSourceState(source);

        if (sampleOffset > 0
            && ((audio::Source::State::Playing == state)
                || (audio::Source::State::Paused == state))
        )
        {
            std::vector<audio::Buffer> buffers = GetSourceActiveBuffers(source);
            double timeNs = 0.0;
            uint32_t i = 0;

            while (sampleOffset > 0)
            {
                // get buffer
                audio::Buffer const& buffer = buffers.at(i);

                // check how many samples from this buffer we can consume
                uint32_t const consumedSamples = std::min(
                    static_cast<uint32_t>(sampleOffset)
                    , buffer.GetSampleCount()
                );

                // calculate time based on this buffer properties
                timeNs += (static_cast<double>(consumedSamples)
                        / static_cast<double>(buffer.GetFrequencyHz()))
                    * 1e9;

                // remove consumed samples
                sampleOffset -= consumedSamples;
                ++i;
            }

            result = std::chrono::nanoseconds(static_cast<uint64_t>(std::round(timeNs)));
        }
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: get playback position: {:#x}", source, alErr);
    }

    return result;
}

bool SourceCollection::SetSourcePlaybackPosition(SourceHandle source, std::chrono::nanoseconds offset)
{
    LOG_AUDIO->Debug("Source #{}: set playback position {}ns", source, offset.count());

    audio::Buffer buffer = GetSourceActiveBuffer(source);

    ALint sampleOffset = 0;

    {
        std::vector<audio::Buffer> buffers = GetSourceActiveBuffers(source);
        uint64_t offsetNs = offset.count();
        uint32_t i = 0;

        while (offsetNs > 0)
        {
            // get buffer
            audio::Buffer const& buffer = buffers.at(i);

            // check how much time from this buffer we can consume
            uint64_t const consumedTime = std::min(
                offsetNs
                , static_cast<uint64_t>(buffer.GetDuration().count())
            );

            // calculate time based on this buffer properties
            uint32_t const sampleCount = buffer.GetFrequencyHz()
                * (static_cast<double>(consumedTime) * 1e-9);

            sampleOffset += std::min(sampleCount, buffer.GetSampleCount());

            // removed consumed time
            offsetNs -= consumedTime;
            ++i;
        }
    }


    // clear error state
    ALenum alErr = alGetError();

    alSourcei(static_cast<ALuint>(source), AL_SAMPLE_OFFSET, sampleOffset);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set playback position: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

float SourceCollection::GetSourcePlaybackProgress(SourceHandle source) const
{
    float result = 0.0f;

    // clear error state
    ALenum alErr = alGetError();

    ALint sampleOffset;
    alGetSourcei(static_cast<ALuint>(source), AL_SAMPLE_OFFSET, &sampleOffset);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        audio::Source::State const state = GetSourceState(source);

        if (sampleOffset > 0
            && ((audio::Source::State::Playing == state)
                || (audio::Source::State::Paused == state))
        )
        {
            result = static_cast<float>(sampleOffset) / static_cast<float>(m_sourceMeta.at(source).activeSampleCount);
        }
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: get playback progress: {:#x}", source, alErr);
    }

    return result;
}

bool SourceCollection::SetSourcePlaybackProgress(SourceHandle source, float value)
{
    LOG_AUDIO->Debug("Source #{}: set playback progress {}%", source, value);

    audio::Buffer buffer = GetSourceActiveBuffer(source);

    ALint sampleOffset = static_cast<ALint>(std::round(static_cast<float>(m_sourceMeta[source].activeSampleCount) * value));

    // clear error state
    ALenum alErr = alGetError();

    alSourcei(static_cast<ALuint>(source), AL_SAMPLE_OFFSET, sampleOffset);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set playback progress: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

audio::Source::State SourceCollection::GetSourceState(SourceHandle source) const
{
    audio::Source::State state = audio::Source::State::Unknown;

    // clear error state
    ALenum alErr = alGetError();

    ALint alState;
    alGetSourcei(static_cast<ALuint>(source), AL_SOURCE_STATE, &alState);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        using audio::Source;

        switch (alState)
        {
            case AL_INITIAL:
            {
                state = Source::State::Initial;
                break;
            }
            case AL_PLAYING:
            {
                state = Source::State::Playing;
                break;
            }
            case AL_PAUSED:
            {
                state = Source::State::Paused;
                break;
            }
            case AL_STOPPED:
            {
                state = Source::State::Stopped;
                break;
            }
            default:
            {
                state = Source::State::Unknown;
                break;
            }
        }
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: get state: {:#x}", source, alErr);
    }

    return state;
}

audio::Source::Type SourceCollection::GetSourceType(SourceHandle source) const
{
    audio::Source::Type type = audio::Source::Type::Unknown;

    // clear error type
    ALenum alErr = alGetError();

    ALint alType;
    alGetSourcei(static_cast<ALuint>(source), AL_SOURCE_TYPE, &alType);

    alErr = alGetError();

    if (AL_NO_ERROR == alErr)
    {
        using audio::Source;

        switch (alType)
        {
            case AL_UNDETERMINED:
            {
                type = Source::Type::Undetermined;
                break;
            }
            case AL_STATIC:
            {
                type = Source::Type::Static;
                break;
            }
            case AL_STREAMING:
            {
                type = Source::Type::Streaming;
                break;
            }
            default:
            {
                type = Source::Type::Unknown;
                break;
            }
        }
    }
    else
    {
        LOG_AUDIO->Warning("Source #{}: get type: {:#x}", source, alErr);
    }

    return type;
}

bool SourceCollection::IsSourceRelative(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALint result = AL_FALSE;

    alGetSourcei(static_cast<ALuint>(source), AL_SOURCE_RELATIVE, &result);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        result = AL_FALSE;
        LOG_AUDIO->Warning("Source #{}: get relative: {:#x}", source, alErr);
    }

    return result == AL_TRUE;
}

bool SourceCollection::SetSourceRelative(SourceHandle source, bool flag)
{
    LOG_AUDIO->Debug("Source #{}: set relative {}", source, flag);

    // clear error state
    ALenum alErr = alGetError();

    alSourcei(static_cast<ALuint>(source), AL_SOURCE_RELATIVE, (flag ? AL_TRUE : AL_FALSE));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set relative: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

bool SourceCollection::IsSourceLooping(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALint result = AL_FALSE;

    alGetSourcei(static_cast<ALuint>(source), AL_LOOPING, &result);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        result = AL_FALSE;
        LOG_AUDIO->Warning("Source #{}: get looping: {:#x}", source, alErr);
    }

    return result == AL_TRUE;
}

bool SourceCollection::SetSourceLooping(SourceHandle source, bool flag)
{
    LOG_AUDIO->Debug("Source #{}: set looping {}", source, flag);

    // clear error state
    ALenum alErr = alGetError();

    alSourcei(static_cast<ALuint>(source), AL_LOOPING, (flag ? AL_TRUE : AL_FALSE));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set looping: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

float SourceCollection::GetSourcePitch(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALfloat result = 0.0f;

    alGetSourcef(static_cast<ALuint>(source), AL_PITCH, &result);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        result = -1.0f;
        LOG_AUDIO->Warning("Source #{}: get pitch: {:#x}", source, alErr);
    }

    return result;
}

bool SourceCollection::SetSourcePitch(SourceHandle source, float value)
{
    LOG_AUDIO->Debug("Source #{}: set pitch {}", source, value);

    // clear error state
    ALenum alErr = alGetError();

    alSourcef(static_cast<ALuint>(source), AL_PITCH, value);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set pitch: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

float SourceCollection::GetSourceGain(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALfloat result = 0.0f;

    alGetSourcef(static_cast<ALuint>(source), AL_GAIN, &result);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        result = -1.0f;
        LOG_AUDIO->Warning("Source #{}: get gain: {:#x}", source, alErr);
    }

    return result;
}

bool SourceCollection::SetSourceGain(SourceHandle source, float value)
{
    LOG_AUDIO->Debug("Source #{}: set gain {}", source, value);

    // clear error state
    ALenum alErr = alGetError();

    alSourcef(static_cast<ALuint>(source), AL_GAIN, value);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set gain: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

std::array<float, 3> SourceCollection::GetSourcePosition(SourceHandle source) const
{
    // clear error state
    ALenum alErr = alGetError();

    ALfloat x;
    ALfloat y;
    ALfloat z;

    alGetSource3f(static_cast<ALuint>(source), AL_POSITION, &x, &y, &z);

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: get position: {:#x}", source, alErr);

        x = std::numeric_limits<float>::quiet_NaN();
        y = std::numeric_limits<float>::quiet_NaN();
        z = std::numeric_limits<float>::quiet_NaN();
    }

    return {{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }};
}

bool SourceCollection::SetSourcePosition(SourceHandle source, std::array<float, 3> const& vec)
{
    LOG_AUDIO->Debug("Source #{}: set position {{ {}, {}, {} }}", source, vec[0], vec[1], vec[2]);

    // clear error state
    ALenum alErr = alGetError();

    alSource3f(static_cast<ALuint>(source), AL_POSITION, static_cast<ALfloat>(vec[0]), static_cast<ALfloat>(vec[1]), static_cast<ALfloat>(vec[2]));

    alErr = alGetError();

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: set position: {:#x}", source, alErr);
    }

    return AL_NO_ERROR == alErr;
}

audio::Source SourceCollection::CreateObject(SourceHandle source)
{
    assert(m_objects.cend() == m_objects.find(source));

    m_sourceBuffers.erase(source);
    m_sourceQueuedBuffers.erase(source);

    return audio::Source(std::make_shared<SourceHandle>(source)
        , std::make_shared<SourceCollection*>(const_cast<SourceCollection*>(this))
    );
}

}
}
