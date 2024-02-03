#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if MTA_WIN

#include <Shared/sdk/SharedUtils/utf8.hpp>

#include <vendors/detours/src/detours.h>

#include <shellapi.h>
#include <cassert>

#define HOOKVAR(name) \
    using FUNC_##name = decltype(&name); \
    FUNC_##name pfn##name

#define ADDHOOK(module,name) \
    static_assert(std::is_same_v<decltype(pfn##name), decltype(&name)>, "invalid type of " MTA_STR(pfn##name)); \
    pfn##name = reinterpret_cast<decltype(pfn##name)>(DetourFindFunction(module, #name)); \
    DetourAttach(&reinterpret_cast<PVOID&>(pfn##name), My##name); \
    assert(pfn##name)

namespace NightMTA::SharedUtil::Windows::Detours {
    HOOKVAR(CreateFileA);
    HOOKVAR(LoadLibraryA);
    HOOKVAR(LoadLibraryExA);
    HOOKVAR(SetDllDirectoryA);
    HOOKVAR(SetCurrentDirectoryA);
    HOOKVAR(AddFontResourceExA);
    HOOKVAR(RemoveFontResourceExA);
    HOOKVAR(RemoveDirectoryA);
    HOOKVAR(GetDiskFreeSpaceExA);
    HOOKVAR(GetFileAttributesA);
    HOOKVAR(SetFileAttributesA);
    HOOKVAR(ShellExecuteA);
    HOOKVAR(CreateDirectoryA);
    HOOKVAR(CopyFileA);
    HOOKVAR(MoveFileA);
    HOOKVAR(DeleteFileA);
    HOOKVAR(GetModuleHandleA);

    void AddUtf8FileHooks() noexcept {

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        ADDHOOK("Kernel32.dll", CreateFileA);
        ADDHOOK("Kernel32.dll", LoadLibraryA);
        ADDHOOK("Kernel32.dll", LoadLibraryExA);
        ADDHOOK("Kernel32.dll", SetDllDirectoryA);
        ADDHOOK("Kernel32.dll", SetCurrentDirectoryA);
        ADDHOOK("Gdi32.dll", AddFontResourceExA);
        ADDHOOK("Gdi32.dll", RemoveFontResourceExA);
        ADDHOOK("Kernel32.dll", RemoveDirectoryA);
        ADDHOOK("Kernel32.dll", GetDiskFreeSpaceExA);
        ADDHOOK("Kernel32.dll", GetFileAttributesA);
        ADDHOOK("Kernel32.dll", SetFileAttributesA);
        ADDHOOK("Shell32.dll", ShellExecuteA);
        ADDHOOK("Kernel32.dll", CreateDirectoryA);
        ADDHOOK("Kernel32.dll", CopyFileA);
        ADDHOOK("Kernel32.dll", MoveFileA);
        ADDHOOK("Kernel32.dll", DeleteFileA);
        ADDHOOK("Kernel32.dll", GetModuleHandleA);

        DetourTransactionCommit();
    }
    void RemoveUtf8FileHooks() noexcept {
        #define DELHOOK(name) \
            if (pfn##name) \
            { \
                DetourDetach(&reinterpret_cast<PVOID&>(pfn##name), My##name); \
                pfn##name = nullptr; \
            }

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DELHOOK(CreateFileA)
        DELHOOK(LoadLibraryA)
        DELHOOK(LoadLibraryExA)
        DELHOOK(SetDllDirectoryA)
        DELHOOK(SetCurrentDirectoryA)
        DELHOOK(AddFontResourceExA)
        DELHOOK(RemoveFontResourceExA)
        DELHOOK(RemoveDirectoryA)
        DELHOOK(GetDiskFreeSpaceExA)
        DELHOOK(GetFileAttributesA)
        DELHOOK(SetFileAttributesA)
        DELHOOK(ShellExecuteA)
        DELHOOK(CreateDirectoryA)
        DELHOOK(CopyFileA)
        DELHOOK(MoveFileA)
        DELHOOK(DeleteFileA)
        DELHOOK(GetModuleHandleA)

        DetourTransactionCommit();
    }

    HANDLE WINAPI MyCreateFileA(
        __in LPCSTR lpFileName, __in DWORD dwDesiredAccess, __in DWORD dwShareMode,
        __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, __in DWORD dwCreationDisposition,
        __in DWORD dwFlagsAndAttributes, __in_opt HANDLE hTemplateFile
    ) noexcept {
        std::string strFileName = lpFileName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strFileName = MakeSurePathIsUTF8(strFileName);
#endif
        return CreateFileW(UTF8::fromUTF8(strFileName), dwDesiredAccess, dwShareMode,
            lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile
        );
    }
    HMODULE WINAPI MyLoadLibraryA(__in LPCSTR lpLibFileName) noexcept {
        return LoadLibraryW(UTF8::fromUTF8(lpLibFileName));
    }

    HMODULE WINAPI MyLoadLibraryExA(
        __in LPCSTR lpLibFileName, __reserved HANDLE hFile, __in DWORD dwFlags
    ) noexcept { return LoadLibraryExW(UTF8::fromUTF8(lpLibFileName), hFile, dwFlags);
    }

    BOOL WINAPI MySetDllDirectoryA(__in_opt LPCSTR lpPathName) noexcept {
        return SetDllDirectoryW(UTF8::fromUTF8(lpPathName));
    }

    BOOL WINAPI MySetCurrentDirectoryA(__in LPCSTR lpPathName) noexcept {
        std::string strPathName = lpPathName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strPathName = MakeSurePathIsUTF8(strPathName);
#endif
        return SetCurrentDirectoryW(UTF8::fromUTF8(strPathName));
    }

    int WINAPI MyAddFontResourceExA(
        __in LPCSTR name, __in DWORD fl, __reserved PVOID res
    ) noexcept {
        return AddFontResourceExW(UTF8::fromUTF8(name), fl, res);
    }

    BOOL WINAPI MyRemoveFontResourceExA(
        __in LPCSTR name, __in DWORD fl, __reserved PVOID pdv
    ) noexcept {
        return RemoveFontResourceExW(UTF8::fromUTF8(name), fl, pdv);
    }

    BOOL WINAPI MyRemoveDirectoryA(__in LPCSTR lpPathName) noexcept {
        std::string strPathName = lpPathName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strPathName = MakeSurePathIsUTF8(lpPathName);
#endif
        return RemoveDirectoryW(UTF8::fromUTF8(strPathName));
    }

    BOOL WINAPI MyGetDiskFreeSpaceExA(
        __in_opt LPCSTR lpDirectoryName,
        __out_opt PULARGE_INTEGER lpFreeBytesAvailableToCaller,
        __out_opt PULARGE_INTEGER lpTotalNumberOfBytes,
        __out_opt PULARGE_INTEGER lpTotalNumberOfFreeBytes
    ) noexcept {
        return GetDiskFreeSpaceExW(
            UTF8::fromUTF8(lpDirectoryName),
            lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes
        );
    }

    DWORD WINAPI MyGetFileAttributesA(__in LPCSTR lpFileName) noexcept {
        std::string strFileName = lpFileName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strFileName = MakeSurePathIsUTF8(strFileName);
#endif
        return GetFileAttributesW(UTF8::fromUTF8(strFileName));
    }

    BOOL WINAPI MySetFileAttributesA(__in LPCSTR lpFileName, __in DWORD dwFileAttributes) {
        std::string strFileName = lpFileName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strFileName = MakeSurePathIsUTF8(strFileName);
#endif
        return SetFileAttributesW(UTF8::fromUTF8(strFileName), dwFileAttributes);
    }

    HINSTANCE STDAPICALLTYPE MyShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile,
        LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd
    ) noexcept {
        return ShellExecuteW(
            hwnd, UTF8::fromUTF8(lpOperation), UTF8::fromUTF8(lpFile),
            UTF8::fromUTF8(lpParameters), UTF8::fromUTF8(lpDirectory), nShowCmd
        );
    }

    BOOL WINAPI MyCreateDirectoryA(
        __in LPCSTR lpPathName,
        __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ) noexcept {
        std::string strPathName = lpPathName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strPathName = MakeSurePathIsUTF8(strPathName);
#endif
        return CreateDirectoryW(UTF8::fromUTF8(strPathName), lpSecurityAttributes);
    }

    BOOL WINAPI MyCopyFileA(
        __in LPCSTR lpExistingFileName,
        __in LPCSTR lpNewFileName,
        __in BOOL bFailIfExists
    ) noexcept {
        return CopyFileW(
            UTF8::fromUTF8(lpExistingFileName), UTF8::fromUTF8(lpNewFileName),
            bFailIfExists
        );
    }

    BOOL WINAPI MyMoveFileA(
        __in LPCSTR lpExistingFileName,
        __in LPCSTR lpNewFileName
    ) noexcept {
        return MoveFileW(UTF8::fromUTF8(lpExistingFileName), UTF8::fromUTF8(lpNewFileName));
    }

    BOOL WINAPI MyDeleteFileA(__in LPCSTR lpFileName) noexcept {
        std::string strFileName = lpFileName;
#ifdef MTA_CLIENT
        if (IsGTAProcess())
            strFileName = MakeSurePathIsUTF8(strFileName);
#endif
        return DeleteFileW(UTF8::fromUTF8(strFileName));
    }

    HMODULE WINAPI MyGetModuleHandleA(__in_opt LPCSTR lpModuleName) noexcept {
        return GetModuleHandleW(lpModuleName ? UTF8::fromUTF8(lpModuleName) : nullptr);
    }
}

#endif