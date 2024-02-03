#pragma once

#include <string>

namespace NightMTA::Shared::Core {
    class IModManager;
}

namespace NightMTA::Server::Core {
    class CNetServer {};
    class CXML {};

    class IServer {
    public:
        virtual       CNetServer*                GetNetwork() noexcept = 0;
        virtual       Shared::Core::IModManager* GetModManager() noexcept = 0;
        virtual       CXML*                      GetXML() noexcept = 0;

        virtual const CNetServer*                GetNetwork() const noexcept = 0;
        virtual const Shared::Core::IModManager* GetModManager() const noexcept = 0;
        virtual const CXML*                      GetXML() const noexcept = 0;

        virtual const char* GetServerModPath() const noexcept = 0;

        virtual       std::string GetAbsolutePath(const char* szRelative) noexcept = 0;
        virtual const std::string GetAbsolutePath(const char* szRelative) const noexcept = 0;

        virtual void Printf(const char* szFormat, ...) const noexcept = 0;
        virtual bool IsRequestingExit() const noexcept = 0;

        // Clears input buffer
        virtual bool ClearInput() noexcept = 0;
        // Prints current input buffer on a new line, clears the input buffer and resets history selection
        virtual bool ResetInput() noexcept = 0;
    };

    class IServerBase {
    public:
        virtual void ServerInitialize(IServer* pServer) noexcept = 0;
        virtual bool ServerStartup(int iArgumentCount, char* szArguments[]) noexcept = 0;
        virtual void ServerShutdown() noexcept = 0;

        virtual void DoPulse() noexcept = 0;
        virtual void HandleInput(char* szCommand) noexcept = 0;
        virtual void GetTag(char* szInfoTag, int iInfoTag) const noexcept = 0;

        virtual bool IsReadyToAcceptConnections() const noexcept = 0;
        virtual bool IsFinished() const noexcept = 0;
        virtual bool PendingWorkToDo() const noexcept = 0;
        virtual bool GetSleepIntervals(int& iSleepBusyMs, int& iSleepIdleMs, int& iLogicFpsLimit) const noexcept = 0;
    };

}