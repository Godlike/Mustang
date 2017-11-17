/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_AUDIO_SOURCE_HPP
#define TULPAR_AUDIO_SOURCE_HPP

#include <AL/al.h>
#include <AL/alc.h>
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#include <iostream>
#include <string>

namespace tulpar
{

class AudioSource
{
public:
    AudioSource();

    ~AudioSource();

    bool Load(std::string const& filename);

    void Play()
    {
        alSourcePlay(m_source);
        ALint source_state;
        alGetSourcei(m_source, AL_SOURCE_STATE, &source_state);

        while (source_state == AL_PLAYING)
        {
            alGetSourcei(m_source, AL_SOURCE_STATE, &source_state);
        }
    }
private:
    stb_vorbis* m_audioFile;
    stb_vorbis_info m_audioFileInfo;
    unsigned int m_alBuffer;
    ALuint m_source;

};

}

#endif // TULPAR_AUDIO_SOURCE_HPP
