#pragma once

#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::SharedUtil::UTF {
    constexpr SString UTF16ToUTF8(const std::wstring& wstr) noexcept {
        SString result;
        result.reserve(wstr.length());

        for (const auto& wchar : wstr) {
            if (wchar <= 0x7f) {
                // ASCII character
                result.push_back(static_cast<char>(wchar));
            } else if (wchar <= 0x7ff) {
                // 2-byte character
                result.push_back(0xc0 | ((wchar >> 6) & 0x1f));
                result.push_back(0x80 | (wchar & 0x3f));
            } else if (wchar <= 0xffff) {
                // 3-byte character
                result.push_back(0xe0 | ((wchar >> 12) & 0x0f));
                result.push_back(0x80 | ((wchar >> 6) & 0x3f));
                result.push_back(0x80 | (wchar & 0x3f));
            } else if (wchar <= 0x10ffff) {
                // 4-byte character
                result.push_back(0xf0 | ((wchar >> 18) & 0x07));
                result.push_back(0x80 | ((wchar >> 12) & 0x3f));
                result.push_back(0x80 | ((wchar >> 6) & 0x3f));
                result.push_back(0x80 | (wchar & 0x3f));
            }
        }

        return result;
    }

    constexpr std::wstring UTF8ToUTF16(const SString& str) {
        std::wstring result;

        for (size_t i = 0; i < str.length(); ++i) {
            wchar_t wchar;
            std::uint8_t ch = static_cast<std::uint8_t>(str[i]);
            
            if ((ch & 0x80) == 0) {
                // ASCII character
                wchar = ch;
            } else if ((ch & 0xe0) == 0xc0) {
                // 2-byte character
                wchar = (ch & 0x1f) << 6;
                wchar |= (str[++i] & 0x3f);
            } else if ((ch & 0xf0) == 0xe0) {
                // 3-byte character
                wchar = (ch & 0x0f) << 12;
                wchar |= (static_cast<std::uint8_t>(str[++i]) & 0x3f) << 6;
                wchar |= (static_cast<std::uint8_t>(str[++i]) & 0x3f);
            } else if ((ch & 0xf8) == 0xf0) {
                // 4-byte character
                wchar = (ch & 0x07) << 18;
                wchar |= (static_cast<std::uint8_t>(str[++i]) & 0x3f) << 12;
                wchar |= (static_cast<std::uint8_t>(str[++i]) & 0x3f) << 6;
                wchar |= (static_cast<std::uint8_t>(str[++i]) & 0x3f);
            }
            result.push_back(wchar);
        }

        return result;
    }

    std::string toUTF8(const char* string) noexcept {
        const auto length = strlen(string);
    }
}