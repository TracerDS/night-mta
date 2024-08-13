#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <string>
#include <cstdlib>
#include <Windows.h>

namespace NightMTA::Shared::UTF8 {
    constexpr std::wstring FromUTF8(const std::wstring& string) noexcept { return string; }
    inline std::wstring FromUTF8(const std::string& string) noexcept {
        std::wstring data;
        if(string.empty())
            return data;

        const auto stringSize = string.size();
#if MTA_WIN
        const auto size = MultiByteToWideChar(
            CP_UTF8, 0, string.c_str(), static_cast<int>(stringSize),
            nullptr, 0
        );
        data.resize(size + 1);
        MultiByteToWideChar(
            CP_UTF8, 0, string.c_str(), static_cast<int>(stringSize),
            data.data(), size
        );
#else
        const auto size = std::mbstowcs(nullptr, string.c_str(), stringSize) + 1;
        data.resize(size + 1);
        std::mbstowcs(data.data(), string.c_str(), stringSize);
#endif
        return data;
    }
    constexpr std::string ToUTF8(const std::string& string) noexcept { return string; }
    inline std::string ToUTF8(const std::wstring& string) noexcept {
        std::string data;
        if(string.empty())
            return data;

        const auto stringSize = string.size();
#if MTA_WIN
        const auto size = WideCharToMultiByte(
            CP_UTF8, 0, string.c_str(), static_cast<int>(stringSize),
            nullptr, 0, nullptr, nullptr
        );

        data.resize(size);
        WideCharToMultiByte(
            CP_UTF8, 0, string.c_str(), static_cast<int>(stringSize),
            data.data(), size, nullptr, nullptr
        );
#else
        const auto size = std::wcstombs(nullptr, string.c_str(), stringSize) + 1;
        data.resize(size);
        std::wcstombs(data.data(), string.c_str(), stringSize);
#endif
        return data;
    }
}
