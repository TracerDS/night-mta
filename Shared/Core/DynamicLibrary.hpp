#pragma once

#include "WindowsHeader.hpp"

#ifndef _WIN32
using HMODULE = void*;
#endif

namespace NightMTA::Shared {
    class DynamicLibrary {
        HMODULE m_hModule{ nullptr };
    public:
        using ProcAddr = void(*)();

        DynamicLibrary() noexcept;
        ~DynamicLibrary() noexcept;

        bool Load(const char* path) noexcept;

        void Unload() noexcept;
        bool IsLoaded() const noexcept;

        ProcAddr GetProcAddr(const char* szProcName) noexcept;
    };
}