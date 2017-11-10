/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <tulpar/OpenALContext.hpp>

int main(int argc, char **argv)
{
    tulpar::OpenALContext ctx;

    if(!ctx.Init())
    {
        return -1;
    }

    ctx.Deinit();
}

