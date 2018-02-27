/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/Collection.hpp>

namespace tulpar
{
namespace internal
{

// BufferCollection::

BufferCollection::BufferCollection()
    : Collection<audio::Buffer>()
{

}

BufferCollection::~BufferCollection()
{

}

void BufferCollection::SetBufferName(Handle handle, std::string const& name)
{
    m_bufferNames[handle] = name;
}

std::string BufferCollection::GetBufferName(Handle handle) const
{
    auto nameIt = m_bufferNames.find(handle);

    return ((m_bufferNames.cend() != nameIt) ? nameIt->second : std::string());
}

bool BufferCollection::SetBufferData(Handle /*handle*/, mule::asset::Content const& /*content*/)
{
    return true;
}

bool BufferCollection::ResetBuffer(Handle handle)
{
    Reclaim(handle);

    m_bufferNames.erase(handle);

    return true;
}

void BufferCollection::GenerateHandles(uint32_t batchSize)
{
    m_used.reserve(m_used.size() + batchSize);

    Handle handle = m_used.size();

    for (uint32_t i = 0; i < batchSize; ++i)
    {
        m_available.push(handle++);
    }
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

// SourceCollection::

SourceCollection::SourceCollection(BufferCollection const& buffers)
    : Collection<audio::Source>()
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

void SourceCollection::GenerateHandles(uint32_t batchSize)
{
    m_used.reserve(m_used.size() + batchSize);

    SourceHandle handle = m_used.size();

    for (uint32_t i = 0; i < batchSize; ++i)
    {
        m_available.push(handle++);
    }
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
