#pragma once

namespace NightMTA::Shared::Core {
    class IModManager {
    public:
        virtual bool RequestLoad(const char* szModName) noexcept = 0;

        virtual const char* GetServerPath() const noexcept = 0;
        virtual const char* GetModPath() const noexcept = 0;
        virtual std::string GetAbsolutePath(const char* szRelative) const noexcept = 0;

        virtual void SetExitCode(int exitCode) noexcept = 0;
        virtual int  GetExitCode() const noexcept = 0;
    };
}