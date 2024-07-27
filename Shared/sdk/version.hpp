#pragma once

#include "SharedUtils/Defines.hpp"

#if !MTA_CPP20
#   error "C++ version is invalid!"
#endif

#include <string_view>
#include <string>
#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>

namespace NightMTA::Shared::Version {
    inline const std::string MTA_NAME = "NightMTA";

    constexpr std::uint8_t VERSION_MAJOR = 1;
    constexpr std::uint8_t VERSION_MINOR = 6;
    constexpr std::uint8_t VERSION_RELEASE = 0;

    inline const std::string_view MTA_VERSION = std::to_string(VERSION_MAJOR) + "."
        + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_RELEASE);

    namespace VersionTypes {
        constexpr std::uint8_t CUSTOM = 0b0001;
        constexpr std::uint8_t EXPERIMENTAL = 0b0011;
        constexpr std::uint8_t UNSTABLE = 0b0101;
        constexpr std::uint8_t UNTESTED = 0b0111;
        constexpr std::uint8_t RELEASE = 0b1001;
    }
    constexpr std::uint8_t VERSION_TYPE = VersionTypes::CUSTOM;

#ifdef MTA_CLIENT
    inline const std::string MTA_FULL_STRING = MTA_NAME + " " + "Client";
#else
    inline const std::string MTA_FULL_STRING = MTA_NAME + " " + "Server";
#endif
}


#ifdef MTA_CLIENT
#   define MTA_DM_FULL_STRING "MTA:SA Client"
#else
#   define MTA_DM_FULL_STRING "MTA:SA Server"
#endif
