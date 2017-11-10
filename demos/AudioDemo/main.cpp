/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/OpenALContext.hpp>
#include <tulpar/AudioSource.hpp>

int main(int argc, char **argv)
{
    tulpar::OpenALContext ctx;

    if(!ctx.Init())
    {
        return -1;
    }

    tulpar::AudioSource* as = ctx.SpawnSource("data/back.ogg");

    ctx.Deinit();
}

