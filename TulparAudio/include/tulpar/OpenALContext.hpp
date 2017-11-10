/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef OPENAL_CONTEXT_HPP
#define OPENAL_CONTEXT_HPP

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>

namespace tulpar
{

class OpenALContext
{
public:
    OpenALContext() = default;

    ~OpenALContext() = default;

    bool Init();

    void Deinit();
private:
    ALCdevice* m_device;
    ALCcontext* m_context;
};

}

#endif // OPENAL_CONTEXT_HPP
