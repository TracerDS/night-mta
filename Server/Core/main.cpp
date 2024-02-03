#include <Server/Core/Server.hpp>
#include <Server/Core/ErrorCodes.hpp>
#include <Server/Core/main.hpp>

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/utf8Detours.hpp>

#include <string>

using namespace NightMTA::Server;
using namespace NightMTA::SharedUtil;

#ifdef MTA_WIN
Core::ThreadCommandQueue g_CommandQueue;
#endif

MTAEXPORT int Run(int argc, const char* argv[]) noexcept {
    for (auto i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf(MTA_DM_FULL_STRING " v" MTA_DM_BUILDTAG_LONG "\n");
            return 1;
        }
    }

#ifdef MTA_WIN
    SetErrorMode(SEM_FAILCRITICALERRORS);
#endif

#ifdef MTA_x86
    // Apply file hooks if not already done by the client
    bool bSkipFileHooks = false;
    for (int i = 1; i < argc; i++)
        bSkipFileHooks |= std::string(argv[i]).contains("--clientfeedback");

    if (!bSkipFileHooks)
        Windows::Detours::AddUtf8FileHooks();
#endif

    Core::Server server(
#ifdef MTA_WIN
        &g_CommandQueue
#endif
    );

    int iReturn;
    do {
        iReturn = server.Run(argc, argv);
    } while (iReturn == SERVER_RESET_RETURN);

    // Done
#ifdef MTA_x86
    Windows::Detours::RemoveUtf8FileHooks();
#endif

    // Overwrite the exit code, if we are stopping gracefully without errors
    if (iReturn == Core::ERROR_NO_ERROR)
        iReturn = server.GetExitCode();

    return iReturn;
}

MTAEXPORT bool SendServerCommand(const char* szString) noexcept {
    g_CommandQueue.Add(szString);
    return true;
}