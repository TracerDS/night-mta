#pragma once

#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::Server::Launcher {
    class ServerConfig {
    protected:
        SharedUtil::SString m_ip = "127.0.0.1";
        std::uint16_t m_port = 7777;
        std::uint16_t m_maxplayers = 32;
    public:
        const SharedUtil::SString& GetIP() const noexcept { return this->m_ip; }
        const std::uint16_t& GetPort() const noexcept { return this->m_port; }
        const std::uint16_t& GetMaxPlayers() const noexcept { return this->m_maxplayers; }

        void SetIP(const char* ip) noexcept { this->m_ip = ip; }
        void SetPort(const std::uint16_t port) noexcept { this->m_port = port; }
        void SetMaxPlayers(const std::uint16_t maxplayers) noexcept { this->m_maxplayers = maxplayers; }  
    };
}