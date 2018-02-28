/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/SourceCollection.hpp>

#include <tulpar/internal/BufferCollection.hpp>

namespace tulpar
{
namespace internal
{

Collection<audio::Source>::Handles OpenAVSourceHandler::Generate(uint32_t batchSize)
{
    static Collection<audio::Source>::Handle index = 0;

    Collection<audio::Source>::Handles result;

    result.reserve(batchSize);

    for (uint32_t i = 0; i < batchSize; ++i)
    {
        result.push_back(index++);
    }

    return result;
}

void OpenAVSourceHandler::Reclaim(Collection<audio::Source>::Handle /*handle*/)
{

}

SourceCollection::SourceCollection(BufferCollection const& buffers
    , Collection<audio::Source>::HandleGenerator generator
    , Collection<audio::Source>::HandleReclaimer reclaimer
)
    : Collection<audio::Source>(generator, reclaimer)
    , m_buffers(buffers)
{

}

SourceCollection::~SourceCollection()
{

}

void SourceCollection::BindBuffer(SourceHandle source, BufferHandle buffer)
{
    m_sourceBuffers[source] = buffer;
}

audio::Buffer SourceCollection::GetSourceBuffer(SourceHandle source) const
{
    assert(m_sourceBuffers.cend() != m_sourceBuffers.find(source));

    return m_buffers.Get(m_sourceBuffers.at(source));
}

bool SourceCollection::ResetSource(SourceHandle source)
{
    Reclaim(source);

    m_sourceBuffers.erase(source);

    return true;
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
