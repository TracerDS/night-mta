#pragma once

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>

#if MTA_WIN

#include <Shared/Core/WindowsHeader.hpp>

#include <Shared/sdk/SharedUtils/Misc.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::SharedUtil::Windows {
    /**
     * @brief Structure that holds Windows version information.
     *
     * The structure contains major, minor, build number and the full number
     * in a form of a 64-bit integer.
     *
     * The full number can be used for comparison of Windows versions.
     */
    struct WinVer {
        /**
         * Major version number of the Windows.
         */
        std::uint32_t major{ 0 };

        /**
         * Minor version number of the Windows.
         */
        std::uint32_t minor{ 0 };

        /**
         * Build number of the Windows.
         */
        std::uint32_t buildNum{ 0 };

        /**
         * Full version number of the Windows in a form of a 64-bit integer.
         * The full number is calculated as: major * 10000000 + minor * 10000 + buildNum
         */
        std::uint64_t fullNumber{ 0 };

        /**
         * Minimum version number of Windows 7.
         */
        static constexpr std::uint64_t WIN7_MINVER = 6107600;

        /**
         * Minimum version number of Windows 8.
         */
        static constexpr std::uint64_t WIN8_MINVER = 6209200;

        /**
         * Minimum version number of Windows 10.
         */
        static constexpr std::uint64_t WIN10_MINVER = 10010240;

        /**
         * Minimum version number of Windows 11.
         */
        static constexpr std::uint64_t WIN11_MINVER = 10022000;
    };


    /**
     * @brief Get the Windows version number.
     *
     * @return Windows version info
     */
    inline WinVer GetWindowsVersion() noexcept {
        // typedef for RtlGetVersion function
        using RtlGetVersionPtr = HRESULT(WINAPI*)(OSVERSIONINFOA *lpVersionInformation);

        // get RtlGetVersion function from ntdll.dll
        auto RtlGetVersion = Misc::GetProcAddress<RtlGetVersionPtr>("ntdll.dll", "RtlGetVersion");
        if (!RtlGetVersion)
            return {};

        // create OSVERSIONINFOA struct to pass to RtlGetVersion
        OSVERSIONINFOA fow{ 0 };
        fow.dwOSVersionInfoSize = sizeof(fow);

        // call RtlGetVersion and check for failure
        if (RtlGetVersion(&fow) != 0)
            return {};

        // calculate full Windows version number
        const auto fullNumber = fow.dwMajorVersion * 1000000
            + fow.dwMinorVersion * 100000 + fow.dwBuildNumber;

        // return Windows version info
        return { fow.dwMajorVersion, fow.dwMinorVersion, fow.dwBuildNumber, fullNumber };
    }

    /**
     * @brief Check if the current Windows version is Windows 7.
     *
     * @return True if the current Windows version is Windows 7,
     * false otherwise.
     */
    inline bool IsWin7() noexcept {
        const auto winVer = GetWindowsVersion();
        return winVer.fullNumber == WinVer::WIN7_MINVER;
    }

    /**
     * @brief Check if the current Windows version is Windows 8.
     *
     * @return True if the current Windows version is Windows 8,
     * false otherwise.
     */
    inline bool IsWin8() noexcept {
        const auto winVer = GetWindowsVersion();
        return winVer.fullNumber == WinVer::WIN8_MINVER; // Windows 8 minimum version number
    }

    /**
     * @brief Check if the current Windows version is Windows 10.
     *
     * @return True if the current Windows version is Windows 10,
     * false otherwise.
     */
    inline bool IsWin10() noexcept {
        const auto winVer = GetWindowsVersion();
        return winVer.fullNumber == WinVer::WIN10_MINVER; // Windows 10 minimum version number
    }

    /**
     * @brief Check if the current Windows version is Windows 11.
     *
     * @return True if the current Windows version is Windows 11,
     * false otherwise.
     */
    inline bool IsWin11() noexcept {
        const auto winVer = GetWindowsVersion(); // Get the current Windows version
        return winVer.fullNumber == WinVer::WIN11_MINVER; // Check if it's exactly Windows 11
    }

    /**
     * @brief Check if the current Windows version is at least Windows 7.
     *
     * @return True if the current Windows version is at least Windows 7,
     * false otherwise.
     */
    inline bool IsWin7OrGreater() noexcept {
        const auto winVer = GetWindowsVersion(); // Get the current Windows version
        return winVer.fullNumber >= WinVer::WIN7_MINVER; // Check if it's at least Windows 7
    }
    /**
     * @brief Check if the current Windows version is at least Windows 8.
     *
     * @return True if the current Windows version is at least Windows 8,
     * false otherwise.
     */
    inline bool IsWin8OrGreater() noexcept {
        const auto winVer = GetWindowsVersion();
        return winVer.fullNumber >= WinVer::WIN8_MINVER; // Check if it's at least Windows 8
    }
    /**
     * @brief Check if the current Windows version is at least Windows 10.
     *
     * @return True if the current Windows version is at least Windows 10,
     * false otherwise.
     */
    inline bool IsWin10OrGreater() noexcept {
        const auto winVer = GetWindowsVersion(); // Get the current Windows version
        return winVer.fullNumber >= WinVer::WIN10_MINVER; // Check if it's at least Windows 10
    }
    /**
     * @brief Check if the current Windows version is at least Windows 11.
     *
     * @return True if the current Windows version is at least Windows 11,
     * false otherwise.
     */
    inline bool IsWin11OrGreater() noexcept {
        const auto winVer = GetWindowsVersion(); // Get the current Windows version
        return winVer.fullNumber >= WinVer::WIN11_MINVER; // Check if it's at least Windows 11
    }
}
#endif