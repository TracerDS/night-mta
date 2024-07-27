#pragma once

#include "Defines.hpp"

auto a = __cplusplus;

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>

namespace NightMTA::SharedUtil::Misc {
    /**
     * @brief Retrieves a function pointer from a shared library
     *
     * @tparam T Function pointer type
     * @param handle Handle to the shared library
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <typename T>
    inline T GetProcAddress(const char* handle, const char* name) noexcept {
#if MTA_WIN
        // Retrieve the module handle
        const auto moduleHandle = GetModuleHandleA(handle);
        if (!moduleHandle) return reinterpret_cast<T>(0); // Module not found

        // Retrieve function pointer from module
        const auto proc = ::GetProcAddress(moduleHandle, name);
        if (!proc) return reinterpret_cast<T>(0); // Function not found

        return reinterpret_cast<T>(proc); // Return function pointer
#else
        // Open the shared library
        // RTLD_LOCAL: unload library when last reference is closed
        // RTLD_LAZY: load symbols only when they are used
        auto dynamicLib = dlopen(handle, RTLD_LOCAL | RTLD_LAZY);
        if(!dynamicLib) return reinterpret_cast<T>(0); // Could not open shared library

        // Retrieve function pointer from shared library
        auto func = GetProcAddress(dynamicLib, name);

        // Close shared library
        dlclose(dynamicLib);

        // Return function pointer
        return func;
#endif
    }
}