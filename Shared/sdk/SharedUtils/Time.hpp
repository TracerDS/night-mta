#pragma once

#include <ctime>

#include <Shared/sdk/SharedUtils/SString.hpp>

#if !MTA_WIN
#   include <sys/time.h>
#endif

// Windows definition:
// #define GetCurrentTime() GetTickCount()
#ifdef GetCurrentTime
#   undef GetCurrentTime
#endif

namespace NightMTA::Shared::Time {
    struct TimeStruct {
        std::uint8_t hours{ 0 };
        std::uint8_t minutes{ 0 };
        std::uint8_t seconds{ 0 };
        std::uint16_t milliseconds{ 0 };

        std::uint8_t day{ 1 };
        std::uint8_t month{ 1 };
        std::uint16_t year{ 1970 };

        TimeStruct() noexcept {}
        TimeStruct(const std::tm& time) noexcept :
            milliseconds(0), seconds(time.tm_sec), minutes(time.tm_min), hours(time.tm_hour),
            day(time.tm_mday), month(time.tm_mon + 1), year(time.tm_year + 1900) {}
#if MTA_WIN
        TimeStruct(const SYSTEMTIME& time) noexcept :
            milliseconds(static_cast<std::uint16_t>(time.wMilliseconds)),
            seconds(static_cast<std::uint8_t>(time.wSecond)),
            minutes(static_cast<std::uint8_t>(time.wMinute)),
            hours(static_cast<std::uint8_t>(time.wHour)),
            day(static_cast<std::uint8_t>(time.wDay)),
            month(static_cast<std::uint8_t>(time.wMonth)),
            year(static_cast<std::uint16_t>(time.wYear)) {}

        TimeStruct& operator=(const SYSTEMTIME& time) noexcept {
            this->milliseconds = static_cast<std::uint16_t>(time.wMilliseconds);
            this->seconds = static_cast<std::uint8_t>(time.wSecond);
            this->minutes = static_cast<std::uint8_t>(time.wMinute);
            this->hours = static_cast<std::uint8_t>(time.wHour);
            this->day = static_cast<std::uint8_t>(time.wDay);
            this->month = static_cast<std::uint8_t>(time.wMonth);
            this->year = static_cast<std::uint16_t>(time.wYear);
            return *this;
        }
#endif
    };

    inline TimeStruct GetCurrentTime(bool local = true) noexcept {
        TimeStruct currentTime;
#if MTA_WIN
        SYSTEMTIME systemTime;
        local ? GetLocalTime(&systemTime) : GetSystemTime(&systemTime);

        currentTime = systemTime;
#else
        std::time_t tempTime = std::time(nullptr);
        currentTime = local ? std::localtime(&tempTime) : std::gmtime(&tempTime);
        timeval curTime;
        gettimeofday(&curTime, nullptr);
        currentTime.milliseconds = curTime.tv_usec / 1000;
#endif
        return TimeStruct(currentTime);
    }

    inline SString GetCurrentTimeString(bool date = true, bool milliseconds = false, bool local = true) noexcept {
        const auto time = GetCurrentTime(local);

        SString result = "";
        if (date)
            result = SString("%02d-%02d-%04d", time.day, time.month, time.year);

        result += SString("%02d:%02d:%02d", time.hours, time.minutes, time.seconds);

        if (milliseconds)
            result += SString(".%03d", time.milliseconds);

        return result;
    }
}
