/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/OpenALContext.hpp>

namespace tulpar
{

bool OpenALContext::Init()
{
    m_device = alcOpenDevice(NULL);

    if(!m_device)
    {
        std::cerr << "Can't open device\n";
        return false;
    }

    m_context = alcCreateContext(m_device, NULL);

    alcMakeContextCurrent(m_context);

    if(!m_context)
    {
        std::cerr << "Can't create context\n";
        return false;
    }

    std::cout << "OpenAL initialized\n";

    return true;
}

void OpenALContext::Deinit()
{
    alcMakeContextCurrent(NULL);

    if(m_context)
    {
        alcDestroyContext(m_context);
    }

    if(m_device)
    {
        alcCloseDevice(m_device);
    }
    std::cout << "OpenAL deinitialized\n";
}

}
