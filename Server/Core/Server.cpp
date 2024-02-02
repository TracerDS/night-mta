#include <Server/Core/Server.hpp>
#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>

#include <cstring>
#include <cstdio>
#include <detours.h>
#include <string>

MTAEXPORT int Run(int argc, const char* argv[]) noexcept {
    for (auto i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf(MTA_DM_FULL_STRING " v" MTA_DM_BUILDTAG_LONG "\n");
            return 1;
        }
    }
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS);
#endif

#ifdef MTA_x86
    // Apply file hooks if not already done by the client
    bool bSkipFileHooks = false;
    for (int i = 1; i < argc; i++)
        bSkipFileHooks |= std::string(argv[i]).Contains("--clientfeedback");

    if (!bSkipFileHooks)
        AddUtf8FileHooks();
#endif
}