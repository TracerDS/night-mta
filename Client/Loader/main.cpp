#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/EventLog.hpp>

namespace Shared = NightMTA::Shared;

MTAEXPORT int DoWinMain(HINSTANCE hLauncherInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Shared::EventLog::BeginEventLog();
    /*

    // Start localization if possible
    InitLocalization(false);

    // Handle commands from the installer
    HandleSpecialLaunchOptions();

    // Check MTA is launched only once
    HandleDuplicateLaunching();

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
