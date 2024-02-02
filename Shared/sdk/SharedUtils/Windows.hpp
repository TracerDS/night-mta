#pragma once

#ifdef _WIN32
#include <Shared/Core/WindowsHeader.hpp>

namespace SharedUtil::Windows {
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
    bool IsWindowsXPSP3OrGreater() noexcept
    {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
    }

    bool IsWindowsVistaOrGreater() noexcept
    {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
    }

    bool IsWindows7OrGreater() noexcept
    {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
    }

    bool IsWindows8OrGreater() noexcept
    {
        return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
    }

    void AddUtf8FileHooks() noexcept
    {
#define ADDHOOK(module,name) \
    static_assert(std::is_same_v<decltype(pfn##name), decltype(&name)>, "invalid type of " MTA_STR(pfn##name)); \
    pfn##name = reinterpret_cast<decltype(pfn##name)>(DetourFindFunction(module, #name)); \
    DetourAttach(&reinterpret_cast<PVOID&>(pfn##name), My##name); \
    assert(pfn##name);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        ADDHOOK("Kernel32.dll", CreateFileA)
        ADDHOOK("Kernel32.dll", LoadLibraryA)
        ADDHOOK("Kernel32.dll", LoadLibraryExA)
        ADDHOOK("Kernel32.dll", SetDllDirectoryA)
        ADDHOOK("Kernel32.dll", SetCurrentDirectoryA)
        ADDHOOK("Gdi32.dll", AddFontResourceExA)
        ADDHOOK("Gdi32.dll", RemoveFontResourceExA)
        ADDHOOK("Kernel32.dll", RemoveDirectoryA)
        ADDHOOK("Kernel32.dll", GetDiskFreeSpaceExA)
        ADDHOOK("Kernel32.dll", GetFileAttributesA)
        ADDHOOK("Kernel32.dll", SetFileAttributesA)
        ADDHOOK("Shell32.dll", ShellExecuteA)
        ADDHOOK("Kernel32.dll", CreateDirectoryA)
        ADDHOOK("Kernel32.dll", CopyFileA)
        ADDHOOK("Kernel32.dll", MoveFileA)
        ADDHOOK("Kernel32.dll", DeleteFileA)
        ADDHOOK("Kernel32.dll", GetModuleHandleA)

        DetourTransactionCommit();
    }

}
#endif