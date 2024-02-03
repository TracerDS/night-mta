/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CModManagerImpl.h
 *  PURPOSE:     Mod manager class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <Shared/Core/ModManagerInterface.hpp>
#include <Shared/Core/DynamicLibrary.hpp>

namespace NightMTA::Server::Core {
    class Server;
    class IServerBase;

    class ModManager : public Shared::Core::IModManager {
        using InitServer = IServerBase * ();
    public:
        ModManager(Server* pServer) noexcept;
        virtual ~ModManager() noexcept;

        constexpr void SetServerPath(const char* szServerPath) noexcept {
            m_strServerPath = szServerPath;
        }

        bool RequestLoad(const char* szModName) noexcept override;

        const char* GetServerPath() const noexcept override { return m_strServerPath.c_str(); };
        const char* GetModPath() const noexcept override { return m_strModPath.c_str(); };
        std::string GetAbsolutePath(const char* szRelative) const noexcept override;

        bool IsModLoaded() const noexcept;

        IServerBase* GetCurrentMod() noexcept;
        const IServerBase* GetCurrentMod() const noexcept;

        bool Load(const char* szModName, int iArgumentCount, char* szArguments[]) noexcept;
        void Unload(bool bKeyPressBeforeTerm = false) noexcept;

        void HandleInput(const char* szCommand) noexcept;
        void GetTag(char* szInfoTag, int iInfoTag) const noexcept;

        void DoPulse() noexcept;

        bool IsReadyToAcceptConnections() const noexcept;

        bool IsFinished() const noexcept;

        bool PendingWorkToDo();
        bool GetSleepIntervals(int& iSleepBusyMs, int& iSleepIdleMs, int& iLogicFpsLimit);

        constexpr Shared::Core::DynamicLibrary& GetDynamicLibrary() noexcept {
            return m_Library;
        }
        constexpr const Shared::Core::DynamicLibrary& GetDynamicLibrary() const noexcept {
            return m_Library;
        }

        void SetExitCode(int exitCode) noexcept override;
        int  GetExitCode() const noexcept override;

    private:
        Server* m_pServer{ nullptr };

        IServerBase* m_pBase{ nullptr };
        Shared::Core::DynamicLibrary m_Library;

        std::string m_strServerPath; // Root path
        std::string m_strModPath;    // root/mods/deathmatch
    };
}