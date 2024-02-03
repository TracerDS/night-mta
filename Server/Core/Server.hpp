/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CServerImpl.h
 *  PURPOSE:     Server class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <Server/Core/ServerInterface.hpp>
#include <Server/Core/ThreadCommandQueue.hpp>
#include <Server/Core/ErrorCodes.hpp>

#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/sdk/SharedUtils/wstring.hpp>
#include <Shared/Core/DynamicLibrary.hpp>

#include <string>
#include <vector>

#define SERVER_RESET_RETURN 500

class CNetServer;
class CXML;

using InitXMLInterface = CXML*(*)(const char* szSaveFlagDirectory);
using InitNetServerInterface = CNetServer*(*)();

#ifdef MTA_WIN
constexpr SHORT SCREEN_BUFFER_SIZE = 256;
#endif


namespace NightMTA::Server::Core {
    class Server : public IServer {
    public:
        Server(
    #ifdef MTA_WIN
            ThreadCommandQueue* pThreadCommandQueue
    #endif
        ) noexcept;
        ~Server() noexcept;

        CNetServer*                GetNetwork() noexcept override;
        Shared::Core::IModManager* GetModManager() noexcept override;
        CXML*                      GetXML() noexcept override;

        const CNetServer*                GetNetwork() const noexcept override;
        const Shared::Core::IModManager* GetModManager() const noexcept override;
        const CXML*                      GetXML() const noexcept override;

        constexpr const char* GetServerModPath() const noexcept override {
            return m_strServerModPath.c_str(); 
        }

        std::string GetAbsolutePath(const char* szRelative) noexcept override;
        const std::string GetAbsolutePath(const char* szRelative) const noexcept override;

        void Printf(const char* szText, ...) const noexcept;
        bool IsRequestingExit() const noexcept;

        // Clears input buffer
        bool ClearInput() noexcept;
        // Prints current input buffer on a new line, clears the input buffer and resets history selection
        bool ResetInput() noexcept;

        int Run(int iArgumentCount, const char* szArguments[]) noexcept;
    #ifndef MTA_WIN
        void Daemonize() const noexcept;
    #else
        bool HasConsole() const noexcept;
    #endif

        constexpr void SetExitCode(int exitCode) noexcept { m_exitCode = exitCode; }
        constexpr int  GetExitCode() const noexcept { return m_exitCode; }

    private:
        void MainLoop() noexcept;

        bool ParseArguments(int iArgumentCount, const char* szArguments[]) noexcept;

        void ShowInfoTag(char* szTag) noexcept;
        void HandleInput() noexcept;
        void SelectCommandHistoryEntry(std::uint32_t uiEntry) noexcept;
        void HandlePulseSleep() noexcept;
        void ApplyFrameRateLimit(std::uint32_t uiUseRate) noexcept;

        void DestroyWindow() noexcept;

        Shared::Core::DynamicLibrary m_NetworkLibrary;
        Shared::Core::DynamicLibrary m_XMLLibrary;

        CNetServer* m_pNetwork{ nullptr };
        ModManager* m_pModManager{ nullptr };
        CXML*       m_pXML{ nullptr };

        std::string m_strServerPath;
        std::string m_strServerModPath;

        bool m_bRequestedQuit{ false };
        bool m_bRequestedReset{ false };

        wchar_t       m_szInputBuffer[255]{ 0 };
        std::uint32_t m_uiInputCount{ 0 };

        char m_szTag[80]{ 0 };

        double m_dLastTimeMs{ 0 };
        double m_dPrevOverrun{ 0 };

        int m_exitCode{ ERROR_NO_ERROR };

        std::vector<std::vector<SharedUtil::WString>> m_vecCommandHistory{};
        std::uint32_t m_uiSelectedCommandHistoryEntry = 0;

    #ifdef MTA_WIN
        HANDLE    m_hConsole{ nullptr };
        HANDLE    m_hConsoleInput{ nullptr };
        CHAR_INFO m_ScrnBuffer[SCREEN_BUFFER_SIZE];

        ThreadCommandQueue* m_pThreadCommandQueue{ nullptr };
    #endif
    };
}