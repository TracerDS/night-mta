#pragma once

#include <Shared/Core/WindowsHeader.hpp>

#ifndef _WIN32
using HMODULE = void*;
#endif

namespace NightMTA::Shared::Core {
    class IDynamicLibrary {
    protected:
        HMODULE m_hModule;
    public:
        using ProcAddr = void(*)();

        virtual ~IDynamicLibrary() noexcept = 0;

        virtual bool Load(const char* path) noexcept = 0;

        virtual void Unload() noexcept = 0;
        virtual bool IsLoaded() const noexcept = 0;

        virtual ProcAddr GetProcAddr(const char* szProcName) noexcept = 0;
    };

    class DynamicLibrary : public IDynamicLibrary {
    public:
        DynamicLibrary() noexcept;
        ~DynamicLibrary() noexcept override;

        bool Load(const char* path) noexcept override;

        void Unload() noexcept override;
        bool IsLoaded() const noexcept override;

        ProcAddr GetProcAddr(const char* szProcName) noexcept override;
    };
}