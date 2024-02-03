#pragma once

#ifdef _WIN32
#include <Shared/Core/WindowsHeader.hpp>
#include <Shared/sdk/SharedUtils/Defines.hpp>

namespace NightMTA::SharedUtil::Windows {
    /*
    * Thanks godot!
    
    String OS_Windows::get_version() const {
	    RtlGetVersionPtr version_ptr = (RtlGetVersionPtr)GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlGetVersion");
	    if (version_ptr != nullptr) {
		    RTL_OSVERSIONINFOW fow;
		    ZeroMemory(&fow, sizeof(fow));
		    fow.dwOSVersionInfoSize = sizeof(fow);
		    if (version_ptr(&fow) == 0x00000000) {
			    return vformat("%d.%d.%d", (int64_t)fow.dwMajorVersion, (int64_t)fow.dwMinorVersion, (int64_t)fow.dwBuildNumber);
		    }
	    }
	    return "";
    }
    
    */
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