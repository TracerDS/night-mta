#pragma once

#include <cstdint>

namespace NightMTA::Server::Core {
    class INetServer {
    public:
        virtual bool StartNetwork(
            const char* szIP,
            std::uint16_t usServerPort,
            std::uint32_t uiAllowedPlayers,
            const char* szServerName
        ) noexcept = 0;

        virtual void StopNetwork() noexcept = 0;
    };
}