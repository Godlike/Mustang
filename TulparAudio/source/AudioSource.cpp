/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/AudioSource.hpp>
#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

namespace tulpar
{
AudioSource::AudioSource()
    : m_audioFile(nullptr)
{

}

AudioSource::~AudioSource()
{

}

bool AudioSource::Load(std::string const& filename)
{
    m_audioFile = stb_vorbis_open_filename(filename.c_str(), NULL, NULL);

    m_audioFileInfo = stb_vorbis_get_info(m_audioFile);
    int const smpls = stb_vorbis_stream_length_in_samples(m_audioFile) * m_audioFileInfo.channels;
    ALshort* buf = new ::ALshort[smpls];

    alGenBuffers(1, &m_alBuffer);

    stb_vorbis_get_samples_short_interleaved(m_audioFile, m_audioFileInfo.channels, buf, smpls);
    alBufferData(m_alBuffer, ((m_audioFileInfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16),
        buf, (smpls * sizeof(::ALshort)), m_audioFileInfo.sample_rate);

    delete[] buf;
    stb_vorbis_close(m_audioFile);


    alGenSources((ALuint)1, &m_source);

    alSourcef(m_source, AL_PITCH, 1);
    // check for errors
    alSourcef(m_source, AL_GAIN, 1);
    // check for errors
    alSource3f(m_source, AL_POSITION, 0, 0, 0);
    // check for errors
    alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
    // check for errors
    alSourcei(m_source, AL_LOOPING, AL_FALSE);
    // check for errros
    alSourcei(m_source, AL_BUFFER, m_alBuffer);
    return true;
}
}
