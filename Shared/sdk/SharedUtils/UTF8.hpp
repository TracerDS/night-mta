#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <string>
#include <cstdlib>
#include <Shared/sdk/SharedUtils/Windows.hpp>

namespace NightMTA::Shared::UTF8 {
    inline std::wstring FromUTF8(const std::string& string) noexcept {
        std::wstring data;
        if(string.empty())
            return data;

#if MTA_WIN
        const auto size = MultiByteToWideChar(
            CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()),
            nullptr, 0
        );
        data.reserve(size);
        MultiByteToWideChar(
            CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()),
            data.data(), size
        );
#else
        const auto size = std::mbstowcs(nullptr, string.c_str(), string.size()) + 1;
        data.reserve(size);
        std::mbstowcs(data.data(), string.c_str(), string.size());
#endif

        return data;
    }
    inline std::string ToUTF8(const std::wstring& string) noexcept {
        std::string data;
        if(string.empty())
            return data;

#if MTA_WIN
        const auto size = WideCharToMultiByte(
            CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()),
            nullptr, 0, nullptr, nullptr
        );
        data.reserve(size);
        WideCharToMultiByte(
            CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()),
            data.data(), size, nullptr, nullptr
        );
#else
        const auto size = std::wcstombs(nullptr, string.c_str(), string.size()) + 1;
        data.reserve(size);
        std::wcstombs(data.data(), string.c_str(), string.size());
#endif

        return data;
    }
}
