#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/EventLog.hpp>
#include <Shared/sdk/SharedUtils/Machine.hpp>

#include <Shared/sdk/SharedUtils/Crypto.hpp>

#include <Luau/Compiler/include/luacode.h>
#include <Luau/VM/include/lua.h>
#include <Luau/VM/include/lualib.h>

#include "launching.hpp"

namespace Shared = NightMTA::Shared;

MTAEXPORT int DoWinMain(HINSTANCE hLauncherInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Shared::EventLog::BeginEventLog();

    std::size_t bytecodeSize = 0;
    std::string source;

    auto luaVM = luaL_newstate();
    
    char* bytecode = luau_compile(source.c_str(), source.size(), nullptr, &bytecodeSize);
    int result = luau_load(luaVM, "chunkname", bytecode, bytecodeSize, 0);
    delete[] bytecode;
    if (result) {
        Shared::Windows::WinMessageBox(
            std::string("ERROR: ") + lua_tostring(luaVM, -1),
            "Lua Error"
        );
    }

    // Start localization if possible
    // not yet
    //InitLocalization(false);

    // Handle commands from the installer
    // not yet
    //HandleSpecialLaunchOptions();
    
    // Check MTA is launched only once
    //NightMTA::Client::HandleMultiLaunching();

    /*
    // Show logo
    ShowSplash(g_hInstance);

    // Other init stuff
    ClearPendingBrowseToSolution();

    // Find GTA path to use
    ValidateGTAPath();

    //
    // Update
    //

    // Continue any update procedure
    SString strCmdLine = GetInstallManager()->Continue();

    //
    // Launch
    //

    // Ensure localization is started
    InitLocalization(true);

    // Setup/test various counters and flags for monitoring problems
    PreLaunchWatchDogs();

    // Stuff
    HandleCustomStartMessage();
    #if !defined(MTA_DEBUG) && MTASA_VERSION_TYPE != VERSION_TYPE_CUSTOM
    ForbodenProgramsMessage();
    #endif
    CycleEventLog();
    BsodDetectionPreLaunch();
    MaybeShowCopySettingsDialog();

    // Make sure GTA is not running
    HandleIfGTAIsAlreadyRunning();

    // Maybe warn user if no anti-virus running
    CheckAntiVirusStatus();

    // Ensure logo is showing
    ShowSplash(g_hInstance);

    // Check MTA files look good
    CheckDataFiles();
    CheckLibVersions();

    // Go for launch
    int iReturnCode = LaunchGame(strCmdLine);

    PostRunWatchDogs(iReturnCode);

    //
    // Quit
    //

    HandleOnQuitCommand();

    // Maybe show help if trouble was encountered
    ProcessPendingBrowseToSolution();

    AddReportLog(1044, SString("* End (0x%X)* pid:%d", iReturnCode, GetCurrentProcessId()));

    RemoveUtf8FileHooks();
    return iReturnCode;
    */
    return 0;
}
