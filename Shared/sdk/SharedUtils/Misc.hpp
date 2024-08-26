#pragma once

#include <string>
#include <cstdint>
#include <type_traits>
#include <array>

namespace NightMTA::Shared::Misc {
    template <typename... T>
    constexpr auto make_array(T&&... values)
        -> std::array<
        typename std::decay<typename std::common_type<T...>::type>::type,
        sizeof...(T)
        >
    {
        return { std::forward<T>(values)... };
    }

    constexpr const char* HEX_DIGITS = "0123456789ABCDEF";

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

    constexpr std::string BytesToHex(const std::uint8_t* arr, const std::uint64_t size) noexcept {
        std::string hexStr;
        hexStr.resize(size * 2);

        std::uint64_t i = 0;
        while (i < size) {
            hexStr[i++] = HEX_DIGITS[(arr[i] >> 4) & 0xF];
            hexStr[i++] = HEX_DIGITS[arr[i] & 0xF];
        }

        return hexStr;
    }

    constexpr std::string BytesToHex(const std::vector<std::uint8_t>& bytes) noexcept {
        return BytesToHex(bytes.data(), bytes.size());
    }

    template <std::unsigned_integral T>
    constexpr std::string IntToHex(const T& value) noexcept {
        std::string out;
        if (value == 0)
            return "0";

        T copy = value;

        while (copy > 0) {
            out += HEX_DIGITS[copy % 16];
            copy /= 16;
        }

        std::reverse(out.begin(), out.end());

        return out;
    }

    constexpr std::vector<std::uint8_t> HexToBytes(const std::string& hex) noexcept {
        std::vector<std::uint8_t> arr;
        arr.resize(hex.size() / 2);

        for (auto i = 0; i < hex.size(); i += 2) {
            std::uint8_t byte = 0;

            if (hex[i] >= '0' && hex[i] <= '9') {
                byte |= (hex[i] - '0') << 4;
            } else if (hex[i] >= 'a' && hex[i] <= 'f') {
                byte |= (hex[i] - 'a' + 10) << 4;
            } else if (hex[i] >= 'A' && hex[i] <= 'F') {
                byte |= (hex[i] - 'A' + 10) << 4;
            }

            if (hex[i + 1] >= '0' && hex[i + 1] <= '9') {
                byte |= (hex[i + 1] - '0');
            } else if (hex[i + 1] >= 'a' && hex[i + 1] <= 'f') {
                byte |= (hex[i + 1] - 'a' + 10);
            } else if (hex[i + 1] >= 'A' && hex[i + 1] <= 'F') {
                byte |= (hex[i + 1] - 'A' + 10);
            }

            arr.push_back(byte);
        }

        return arr;
    }

    template <typename T>
    constexpr std::uint64_t MemAddressToInt(T value) noexcept {
        return reinterpret_cast<std::uint64_t>(
            reinterpret_cast<std::uint64_t*>(&value)
        );
    }
}
