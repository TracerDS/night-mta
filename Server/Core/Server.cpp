#include <Server/Core/Server.hpp>

bool g_bSilent = false;
bool g_bNoCurses = false;
bool g_bNoTopBar = false;
bool g_bNoCrashHandler = false;

#ifndef MTA_WIN
bool    g_bDaemonized = false;
WINDOW* m_wndMenu = nullptr;
WINDOW* m_wndInput = nullptr;
constexpr bool IsCursesActive() noexcept { return m_wndInput != nullptr; }
#else
bool g_isChildProcess = false;
HANDLE g_readyEvent = nullptr;
#endif

namespace NightMTA::Server::Core {
    constexpr const char* szNetworkLibName = "net" MTA_LIB_SUFFIX MTA_LIB_EXTENSION;
    constexpr const char* szXMLLibName = "xmll" MTA_LIB_SUFFIX MTA_LIB_EXTENSION;

    Server::Server(
#ifdef MTA_WIN
        ThreadCommandQueue* pThreadCommandQueue
#endif
    ) noexcept :
#ifdef MTA_WIN
        m_pThreadCommandQueue(pThreadCommandQueue),
#endif
        m_pModManager(new ModManager(this))
    {}

    Server::~Server() noexcept {
        // Destroy our stuff
        delete m_pModManager;
    }

    bool Server::IsRequestingExit() const noexcept {
    #ifdef MTA_WIN
        // cant be const. Need to move it somewhere
        return m_pThreadCommandQueue->Process(nullptr);
    #endif
        return m_bRequestedQuit;
    }
}