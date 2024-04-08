#pragma once

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/Core/WindowsHeader.hpp>

#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::SharedUtil::Misc {
#if MTA_WIN == 1
    template <class T>
    T GetProcAddress(HMODULE handle, const char* name) noexcept {
        return reinterpret_cast<T>(::GetProcAddress(handle, name));
    }

    template <class T>
    T GetProcAddress(const char* handle, const char* name) noexcept {
        return reinterpret_cast<T>(::GetProcAddress(GetModuleHandleA(handle), name));
    }
#else
#endif    

}