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
    static Context* Create(Device const& device);
    ~Context();

    void MakeCurrent();

private:
    Context();

    bool Initialize(Device const& device);
    void Deinitialize();

    bool m_isInitialized;

    ALCcontext* m_pContext;
    ALCdevice* m_pDevice;
};

}
}

#endif // TULPAR_INTERNAL_CONTEXT_HPP
