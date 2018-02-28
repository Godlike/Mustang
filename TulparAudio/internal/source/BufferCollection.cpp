/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/internal/BufferCollection.hpp>

namespace tulpar
{
namespace internal
{

Collection<audio::Buffer>::Handles OpenAVBufferHandler::Generate(uint32_t batchSize)
{
    static Collection<audio::Buffer>::Handle index = 0;

    Collection<audio::Buffer>::Handles result;

    result.reserve(batchSize);

    for (uint32_t i = 0; i < batchSize; ++i)
    {
        result.push_back(index++);
    }

    return result;
}

void OpenAVBufferHandler::Reclaim(Collection<audio::Buffer>::Handle /*handle*/)
{

}

BufferCollection::BufferCollection(
    Collection<audio::Buffer>::HandleGenerator generator
    , Collection<audio::Buffer>::HandleReclaimer reclaimer
)
    : Collection<audio::Buffer>(generator, reclaimer)
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
