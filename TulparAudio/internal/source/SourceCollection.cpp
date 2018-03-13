/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/internal/BufferCollection.hpp>

#include <tulpar/InternalLoggers.hpp>

#include <AL/al.h>

#include <algorithm>

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

SourceCollection::~SourceCollection()
{

}

void SourceCollection::BindBuffer(SourceHandle source, BufferHandle buffer)
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

    if (AL_NO_ERROR != alErr)
    {
        LOG_AUDIO->Warning("Source #{}: buffer = #{}: {:#x}", source, buffer, alErr);
    }
}

audio::Buffer SourceCollection::GetSourceBuffer(SourceHandle source) const
{
    assert(m_sourceBuffers.cend() != m_sourceBuffers.find(source));

    return m_buffers.Get(m_sourceBuffers.at(source));
}

bool SourceCollection::ResetSource(SourceHandle source)
{
    LOG_AUDIO->Debug("Source #{}: reset", source);

    Reclaim(source);

    m_sourceBuffers.erase(source);

    return true;
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

audio::Source* SourceCollection::GenerateObject(SourceHandle source) const
{
    return new audio::Source(source, const_cast<SourceCollection*>(this)->shared_from_this());
}

audio::Source* SourceCollection::CreateObject(SourceHandle source)
{
    m_sourceBuffers.erase(source);

    return GenerateObject(source);
}

}
}
