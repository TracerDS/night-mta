#pragma once

#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>

namespace NightMTA::Shared::DynamicLibrary {
    class DynamicLibrary {
    public:
#if MTA_WIN
        using DllHandle = HMODULE;
#else
        using DllHandle = void*;
#endif
        DllHandle m_library{ nullptr };
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
            m_library = dlopen(path.c_str(), RTLD_LOCAL | RTLD_LAZY);
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

        template <typename T>
        T GetFuncAddress(const char* name) noexcept {
            return GetProcAddr<T>(m_library, name);
        }
    };

    template <typename T>
    inline T GetProcAddr(const DynamicLibrary::DllHandle& handle, const char* name) noexcept {
        return reinterpret_cast<T>(::GetProcAddress(handle, name));
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
    template <typename T = void*>
    inline T GetFuncAddress(const char* handle, const char* name) noexcept {
#if MTA_WIN
        // Retrieve the module handle
        const auto moduleHandle = GetModuleHandleA(handle);
        if (!moduleHandle) return 0; // Module not found

        // Retrieve function pointer from module
        return GetProcAddr<T>(moduleHandle, name);
#else
        DynamicLibrary lib(handle);
        if (!lib.IsLoaded()) return 0;
        return lib.GetFuncAddress<T>(name);
#endif
    }
}
