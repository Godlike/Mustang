/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef TULPAR_INTERNAL_CONTEXT_HPP
#define TULPAR_INTERNAL_CONTEXT_HPP

#include <tulpar/internal/Device.hpp>

#include <AL/alc.h>

namespace tulpar
{
namespace internal
{

class Context
{
public:
    Context();
    ~Context();

    bool Initialize(Device& device);
    void Deinitialize();

    void MakeCurrent();

private:
    bool m_isInitialized;

    ALCcontext* m_pContext;
};

}
}

#endif // TULPAR_INTERNAL_CONTEXT_HPP
