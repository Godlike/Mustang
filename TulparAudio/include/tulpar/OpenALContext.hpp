/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_OPENAL_CONTEXT_HPP
#define TULPAR_OPENAL_CONTEXT_HPP

#include <tulpar/AudioSource.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <string>
#include <list>

namespace tulpar
{

class OpenALContext
{
public:
    OpenALContext() = default;

    ~OpenALContext() = default;

    bool Init();

    void Deinit();

    AudioSource* SpawnSource(std::string const& filename);
private:
    ALCdevice* m_device;
    ALCcontext* m_context;
    std::list<AudioSource*> m_sources;
};

}

#endif // TULPAR_OPENAL_CONTEXT_HPP
