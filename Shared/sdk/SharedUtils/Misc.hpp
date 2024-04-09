#pragma once

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/Core/WindowsHeader.hpp>

#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::SharedUtil::Misc {
#if MTA_WIN == 1
    /**
     * @brief Retrieves a function pointer from a loaded module
     *
     * @tparam T Function pointer type
     * @param handle Module handle
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <class T>
    inline T GetProcAddress(HMODULE handle, const char* name) noexcept {
        return reinterpret_cast<T>(::GetProcAddress(handle, name));
    }

    /**
     * @brief Retrieves a function pointer from a loaded module
     *
     * This is a convenience function that combines GetModuleHandleA and GetProcAddress
     *
     * @tparam T Function pointer type
     * @param handle Name of the module to retrieve the function from
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <class T>
    inline T GetProcAddress(const char* handle, const char* name) noexcept {
        // Retrieve the module handle
        const auto moduleHandle = GetModuleHandleA(handle);
        if(!moduleHandle) return reinterpret_cast<T>(0); // Module not found

        // Retrieve function pointer from module
        const auto proc = ::GetProcAddress(moduleHandle, name);
        if(!proc) return reinterpret_cast<T>(0); // Function not found

        return reinterpret_cast<T>(proc); // Return function pointer
    }

#else
    /**
     * @brief Retrieves a function pointer from a shared library
     *
     * @tparam T Function pointer type
     * @param handle Handle to the shared library
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <class T>
    inline T GetProcAddress(void* handle, const char* name) noexcept {
        return reinterpret_cast<T>(::dlsym(handle, name));
    }
    
    /**
     * @brief Retrieves a function pointer from a shared library
     *
     * @tparam T Function pointer type
     * @param handle Handle to the shared library
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <class T>
    inline T GetProcAddress(const char* handle, const char* name) noexcept {
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
    }

#endif    

}