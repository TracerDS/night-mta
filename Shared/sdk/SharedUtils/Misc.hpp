#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>

namespace NightMTA::Shared::Misc {
    class DynamicLibrary {
#if !MTA_WIN
        using HMODULE = void*;
#endif
        HMODULE m_library{ nullptr };
    public:
        constexpr DynamicLibrary() noexcept {}
        DynamicLibrary(const SString& path) noexcept { Load(path); }
        ~DynamicLibrary() noexcept {
            if (!m_library) return;
            Unload();
        }

        constexpr bool IsLoaded() const noexcept { return m_library != nullptr; }
        constexpr HMODULE Handle() const noexcept { return m_library; }

        constexpr operator bool() const noexcept { return IsLoaded(); }
        constexpr operator HMODULE() const noexcept { return Handle(); }

        constexpr HMODULE operator()() const noexcept { return Handle(); }

        bool Load(const SString& path) noexcept {
            if (m_library) Unload();
#if MTA_WIN
            m_library = LoadLibraryA(path.c_str());
#else
            m_library = dlopen(handle, RTLD_LOCAL | RTLD_LAZY);
#endif
            return m_library != nullptr;
        }
        bool Unload() noexcept {
            if (!m_library) return false;
#if MTA_WIN
            FreeLibrary(m_library);
#else
            dlclose(m_library);
#endif
            m_library = nullptr;
            return true;
        }
    };

    /**
     * @brief Retrieves a function pointer from a shared library
     *
     * @tparam T Function pointer type
     * @param handle Handle to the shared library
     * @param name Name of the function to retrieve
     *
     * @return Function pointer of type T or nullptr on error
     */
    template <typename T = void*>
    inline T GetProcAddress(const char* handle, const char* name) noexcept {
#if MTA_WIN
        // Retrieve the module handle
        const auto moduleHandle = GetModuleHandleA(handle);
        if (!moduleHandle) return 0; // Module not found

        // Retrieve function pointer from module
        const auto proc = ::GetProcAddress(moduleHandle, name);
        if (!proc) return 0; // Function not found

        return reinterpret_cast<T>(proc); // Return function pointer
#else
        // Open the shared library
        // RTLD_LOCAL: unload library when last reference is closed
        // RTLD_LAZY: load symbols only when they are used
        auto dynamicLib = dlopen(handle, RTLD_LOCAL | RTLD_LAZY);
        if(!dynamicLib) return 0; // Could not open shared library

        // Retrieve function pointer from shared library
        const auto func = GetProcAddress(dynamicLib, name);

        // Close shared library
        dlclose(dynamicLib);

        // Return function pointer
        return reinterpret_cast<T>(func);
#endif
    }

    template <typename T = void*>
    inline T GetProcAddress(const DynamicLibrary& handle, const char* name) noexcept {
        return reinterpret_cast<T>(GetProcAddress(handle.Handle(), name));
    }
}
