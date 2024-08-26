#pragma once

#define MTA_CLIENT

#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>
#include <Shared/sdk/SharedUtils/File.hpp>
#include <Shared/sdk/SharedUtils/Time.hpp>
#include <Shared/sdk/SharedUtils/Misc.hpp>

#ifdef OutputDebugString
#   undef OutputDebugString
#endif

namespace NightMTA::Shared::EventLog {
    inline void LogFile(const SString& message) noexcept {
        if (message.Contains("\n")) {
            for (const auto& line : message.Split("\n")) {
                LogFile(line);
            }
        }

        auto logFilePath = Path::PathJoin({
            Path::GetCurrentExePath(), "MTA", "logs", "logfile.txt"
        });

        static constexpr auto formatString = "%s - %s";
        SString formattedMsg = SString::Format(
            formatString,
            Time::GetCurrentTimeString(true, false, true).c_str(),
            message.c_str()
        );
        File::AppendFile(logFilePath, formattedMsg);
#ifdef MTA_DEBUG
        OutputDebugStringA(message);
#endif
    }

    inline void BeginEventLog() noexcept {
        LogFile("[BEGIN EVENT LOG]");
    }
}
