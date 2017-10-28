#include <iostream>

#include <al.h>
#include <alc.h>

static void ListAudioDevices(const ALCchar *devices)
{
    const ALCchar *device = devices, *next = devices + 1;
    size_t len = 0;

    fprintf(stdout, "Devices list:\n");
    fprintf(stdout, "----------\n");
    while (device && *device != '\0' && next && *next != '\0') {
        fprintf(stdout, "%s\n", device);
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    fprintf(stdout, "----------\n");
}

int main()
{

    ALCdevice * deviceAL = alcOpenDevice(nullptr);

    if (deviceAL == nullptr)
    {
        std::cerr << "Failed to init OpenAL device. \n";
        return -1;
    }

    ALboolean enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");

    if (enumeration == AL_FALSE)
    {
        std::cerr << "Failed to acquire ALC_ENUMERATION_EXT \n";
        return -1;
    }

    ListAudioDevices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

    ALCcontext* context = alcCreateContext(deviceAL, nullptr);

    if (!alcMakeContextCurrent(context))
    {
        std::cerr << "Can't create OpenAL context \n";
        return -1;
    }

    system("pause");

    return 0;
}
