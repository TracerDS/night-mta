#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>

#if MTA_WIN

#include <Windows.h>

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Misc.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>

#include <Shobjidl.h>

namespace NightMTA::Shared::Windows {
    /**
     * @brief Structure that holds Windows version information.
     *
     * The structure contains major, minor, build number and the full number
     * in a form of a 64-bit integer.
     *
     * The full number can be used for comparison of Windows versions.
     */
    struct WindowsVersion {
        /**
         * Major version number of the Windows.
         */
        const std::uint32_t major{ 0 };

        /**
         * Minor version number of the Windows.
         */
        const std::uint32_t minor{ 0 };

        /**
         * Build number of the Windows.
         */
        const std::uint32_t buildNum{ 0 };

        /**
         * Full version number of the Windows in a form of a 64-bit integer.
         * The full number is calculated as: major * 1000000 + minor * 10000 + buildNum
         */
        const std::uint64_t fullNumber{ 0 };

        constexpr WindowsVersion() noexcept {}
        constexpr WindowsVersion(std::uint64_t fullNumber) noexcept :
            fullNumber(fullNumber),
            major(GetMajorNumber(fullNumber)),
            minor(GetMinorNumber(fullNumber)),
            buildNum(GetBuildNumber(fullNumber))
        {}

        constexpr WindowsVersion(
            std::uint32_t major,
            std::uint32_t minor,
            std::uint32_t buildNum,
            std::uint64_t fullNumber = 0
        ) noexcept : major(major), minor(minor), buildNum(buildNum),
            fullNumber(fullNumber != 0 ?
                fullNumber : major * 10000000 + minor * 10000 + buildNum
            ) {}

        constexpr operator bool() const noexcept {
            if (major == 0) return false;
            if (minor == 0) return false;
            if (buildNum == 0) return false;
            if (fullNumber == 0) return false;
            return true;
        }

        static constexpr std::uint8_t GetMajorNumber(std::uint64_t ver) noexcept {
            return static_cast<std::uint8_t>(ver / 1000000);
        }
        static constexpr std::uint8_t GetMinorNumber(std::uint64_t ver) noexcept {
            return static_cast<std::uint8_t>(
                GetMajorNumber(ver) * 1000000 - (ver / 10000) * 10000
            );
        }
        static constexpr std::uint16_t GetBuildNumber(std::uint64_t ver) noexcept {
            return static_cast<std::uint16_t>(
                ver - (GetMajorNumber(ver) * 1000000 + GetMinorNumber(ver) * 10000)
            );
        }

        constexpr bool operator==(const WindowsVersion other) const noexcept {
            if (this->major != other.major) return false;
            if (this->minor != other.minor) return false;
            return true;
        }
        constexpr bool operator==(const std::uint64_t other) const noexcept {
            if (this->major != GetMajorNumber(other))
                return false;
            if (this->minor != GetMinorNumber(other))
                return false;
            return true;
        }

        constexpr bool operator!=(const WindowsVersion other) const noexcept {
            return !operator==(other);
        }
        constexpr bool operator!=(const std::uint64_t other) const noexcept {
            return !operator==(other);
        }

        /**
         * @brief Get the Windows version number.
         *
         * @return Windows version info
         */
        static WindowsVersion GetWindowsVersion() noexcept {
            if (WindowsVersion::ms_version.fullNumber != 0)
                return WindowsVersion::ms_version;

            // typedef for RtlGetVersion function
            using RtlGetVersionPtr = HRESULT(WINAPI*)(OSVERSIONINFOA *lpVersionInformation);

            // get RtlGetVersion function from ntdll.dll
            const static auto RtlGetVersion = Misc::GetProcAddress<RtlGetVersionPtr>("ntdll.dll", "RtlGetVersion");
            if (!RtlGetVersion)
                return {};

            // create OSVERSIONINFOA struct to pass to RtlGetVersion
            static OSVERSIONINFOA fow{ 0 };
            fow.dwOSVersionInfoSize = sizeof(fow);

            // call RtlGetVersion and check for failure
            if (RtlGetVersion(&fow) != 0)
                return {};

            // calculate full Windows version number
            const static auto fullNumber = fow.dwMajorVersion * 1000000
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
        static bool IsWin7() noexcept {
            return ms_version == WIN7_MINVER;
        }

        /**
         * @brief Check if the current Windows version is Windows 8.
         *
         * @return True if the current Windows version is Windows 8,
         * false otherwise.
         */
        static bool IsWin8() noexcept {
            return ms_version.fullNumber == WIN8_MINVER; // Windows 8 minimum version number
        }

        /**
         * @brief Check if the current Windows version is Windows 10.
         *
         * @return True if the current Windows version is Windows 10,
         * false otherwise.
         */
        static bool IsWin10() noexcept {
            return ms_version.fullNumber == WIN10_MINVER; // Windows 10 minimum version number
        }

        /**
         * @brief Check if the current Windows version is Windows 11.
         *
         * @return True if the current Windows version is Windows 11,
         * false otherwise.
         */
        static bool IsWin11() noexcept {
            return ms_version.fullNumber == WIN11_MINVER; // Check if it's exactly Windows 11
        }

        /**
         * @brief Check if the current Windows version is at least Windows 7.
         *
         * @return True if the current Windows version is at least Windows 7,
         * false otherwise.
         */
        static bool IsWin7OrGreater() noexcept {
            return ms_version.fullNumber >= WIN7_MINVER; // Check if it's at least Windows 7
        }
        /**
         * @brief Check if the current Windows version is at least Windows 8.
         *
         * @return True if the current Windows version is at least Windows 8,
         * false otherwise.
         */
        static bool IsWin8OrGreater() noexcept {
            return ms_version.fullNumber >= WIN8_MINVER; // Check if it's at least Windows 8
        }
        /**
         * @brief Check if the current Windows version is at least Windows 10.
         *
         * @return True if the current Windows version is at least Windows 10,
         * false otherwise.
         */
        static bool IsWin10OrGreater() noexcept {
            return ms_version.fullNumber >= WIN10_MINVER; // Check if it's at least Windows 10
        }
        /**
         * @brief Check if the current Windows version is at least Windows 11.
         *
         * @return True if the current Windows version is at least Windows 11,
         * false otherwise.
         */
        static bool IsWin11OrGreater() noexcept {
            return ms_version.fullNumber >= WIN11_MINVER; // Check if it's at least Windows 11
        }
    protected:
        static const WindowsVersion ms_version;

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
    const WindowsVersion WindowsVersion::ms_version = WindowsVersion::GetWindowsVersion();

    inline int WinMessageBox (
        const std::string_view& text,
        const std::string_view& title,
        int flags = MB_ABORTRETRYIGNORE | MB_ICONSTOP
    ) noexcept {
        return MessageBoxA(nullptr, text.data(), title.data(), flags);
    }
    inline int WinMessageBox(
        const std::string_view& text,
        int flags = MB_ABORTRETRYIGNORE | MB_ICONSTOP
    ) noexcept {
        return WinMessageBox(text, Shared::Version::MTA_NAME, flags);
    }

    inline void SetAppID(const std::wstring& name) noexcept {
        if (SetCurrentProcessExplicitAppUserModelID(name.c_str()) != S_OK)
        {
#ifdef MTA_NO_DEBUG
            WinMessageBox(
                "Cannot set app model ID!",
                Shared::Version::MTA_NAME + " - DEBUG",
                MB_OK | MB_ICONWARNING
            );
#endif
        }
    }
}
#endif
