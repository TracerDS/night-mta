#pragma once

#include <string>
#include <cstdint>
#include <type_traits>

namespace NightMTA::Shared::Misc {
    inline std::int8_t stoi8(const std::string& string) noexcept {
        try {
            return std::stoi(string);
        } catch (...) {
            return 0;
        }
    }
    inline std::int16_t stoi16(const std::string& string) noexcept {
        try {
            return std::stoi(string);
        } catch (...) {
            return 0;
        }
    }
    inline std::int32_t stoi32(const std::string& string) noexcept {
        try {
            return std::stoi(string);
        } catch (...) {
            return 0;
        }
    }
    inline std::int64_t stoi64(const std::string& string) noexcept {
        try {
            return std::stoll(string);
        } catch (...) {
            return 0;
        }
    }

    inline std::uint8_t stoui8(const std::string& string) noexcept {
        try {
            return static_cast<std::uint8_t>(std::stoul(string));
        } catch (...) {
            return 0;
        }
    }
    inline std::uint16_t stoui16(const std::string& string) noexcept {
        try {
            return static_cast<std::uint16_t>(std::stoul(string));
        } catch (...) {
            return 0;
        }
    }
    inline std::uint32_t stoui32(const std::string& string) noexcept {
        try {
            return std::stoul(string);
        } catch (...) {
            return 0;
        }
    }
    inline std::uint64_t stoui64(const std::string& string) noexcept {
        try {
            return std::stoull(string);
        } catch (...) {
            return 0;
        }
    }
}
