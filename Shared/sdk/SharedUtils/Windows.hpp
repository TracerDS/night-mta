#pragma once

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>

#if MTA_WIN == 1

#include <Shared/Core/WindowsHeader.hpp>

#include <Shared/sdk/SharedUtils/Misc.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::SharedUtil::Windows {
    inline SString GetWindowsVersion() noexcept {
        using RtlGetVersionPtr = HRESULT(WINAPI*)(OSVERSIONINFOA *lpVersionInformation);

        auto RtlGetVersion = Misc::GetProcAddress<RtlGetVersionPtr>("ntdll.dll", "RtlGetVersion");
        if (!RtlGetVersion)
            return "";

        OSVERSIONINFOA fow{ 0 };
        fow.dwOSVersionInfoSize = sizeof(fow);

        if (RtlGetVersion(&fow) != 0)
            return "";

        return SString("%d.%d.%d", fow.dwMajorVersion, fow.dwMinorVersion, fow.dwBuildNumber);
    }

    bool IsWindowsVersionOrGreater(
        WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor
    ) noexcept {
        OSVERSIONINFOEXW osvi{0};
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        DWORDLONG const dwlConditionMask = VerSetConditionMask(
            VerSetConditionMask(
                VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
                VER_MINORVERSION, VER_GREATER_EQUAL
            ), VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL
        );

        osvi.dwMajorVersion = wMajorVersion;
        osvi.dwMinorVersion = wMinorVersion;
        osvi.wServicePackMajor = wServicePackMajor;

        return VerifyVersionInfoW(&osvi,
            VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR,
            dwlConditionMask);
    }
    bool IsWindowsXPSP3OrGreater() noexcept {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
    }

    bool IsWindowsVistaOrGreater() noexcept {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
    }

    bool IsWindows7OrGreater() noexcept {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
    }

    bool IsWindows8OrGreater() noexcept {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
    }
}
#endif