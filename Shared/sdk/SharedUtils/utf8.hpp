#pragma once

#include <Shared/sdk/SharedUtils/wstring.hpp>
#include <string>

namespace NightMTA::SharedUtil::UTF8 {
    WString fromUTF8(const std::string& string) noexcept {
        WString wideString;

        for (size_t i = 0; i < string.size(); ++i) {
            wchar_t wideChar;

            if ((string[i] & 0x80) == 0) {
                wideChar = static_cast<wchar_t>(string[i]);
            } else if ((string[i] & 0xE0) == 0xC0) {
                wideChar = static_cast<wchar_t>(
                    ((string[i] & 0x1F) << 6) | (string[i + 1] & 0x3F)
                );
                i += 1;
            } else if ((string[i] & 0xF0) == 0xE0) {
                wideChar = static_cast<wchar_t>(((string[i] & 0x0F) << 12) |
                    ((string[i + 1] & 0x3F) << 6) | (string[i + 2] & 0x3F)
                );
                i += 2;
            } else {
                break;
            }

            wideString.push_back(wideChar);
        }

        return wideString;
    }

    std::string toUTF8(const std::wstring& string) noexcept {
        std::string utf8String;

        for (const wchar_t& wideChar : string) {
            char utf8Char;
            
            if (wideChar <= 0x7F) {
                utf8String.push_back(static_cast<char>(wideChar));
            } else if (wideChar <= 0x7FF) {
                utf8String.push_back(static_cast<char>(0xC0 | ((wideChar >> 6) & 0x1F)));
                utf8String.push_back(static_cast<char>(0x80 | (wideChar & 0x3F)));
            } else if (wideChar <= 0xFFFF) {
                utf8String.push_back(static_cast<char>(0xE0 | ((wideChar >> 12) & 0x0F)));
                utf8String.push_back(static_cast<char>(0x80 | ((wideChar >> 6) & 0x3F)));
                utf8String.push_back(static_cast<char>(0x80 | (wideChar & 0x3F)));
            } else if (wideChar <= 0x10FFFF) {
                utf8String.push_back(static_cast<char>(0xF0 | ((wideChar >> 18) & 0x07)));
                utf8String.push_back(static_cast<char>(0x80 | ((wideChar >> 12) & 0x3F)));
                utf8String.push_back(static_cast<char>(0x80 | ((wideChar >> 6) & 0x3F)));
                utf8String.push_back(static_cast<char>(0x80 | (wideChar & 0x3F)));
            } else {
                break;
            }
        }

        return utf8String;
    }
}