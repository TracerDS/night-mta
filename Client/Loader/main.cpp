#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/EventLog.hpp>
#include <Shared/sdk/SharedUtils/Machine.hpp>

#include <Shared/sdk/SharedUtils/Windows.COM.hpp>
#include <Shared/sdk/SharedUtils/Windows.COM.Map.hpp>

namespace Shared = NightMTA::Shared;

MTAEXPORT int DoWinMain(HINSTANCE hLauncherInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Shared::EventLog::BeginEventLog();
    Shared::SString msg;

    if (false) {
        Shared::Windows::COM::COMWrapper wrapper;
        wrapper.Connect();
        Shared::SString out;
        auto result = wrapper.Query("SELECT * FROM Win32_MotherboardDevice");
        for (const auto& field : result) {
            out += Shared::SString("[NEXT]\n");
            for (const auto& [key, value] : field) {
                out += Shared::SString("%s: %s\n", key.c_str(), value.c_str());
            }
        }
        Shared::File::WriteFile("D:/wmires.txt", out);
        Shared::Windows::WinMessageBox(out);
    }

    if (true) {
        auto ram = Shared::Machine::GetRAMInfo();
        std::uint8_t i = 0;
        for (const auto& stick : ram.GetSticks()) {
            msg = Shared::SString("Name: %s\n"
                "Caption: %s\n"
                "Description: %s\n"
                "Speed: %d\n"
                "ClockSpeed: %d\n"
                "MinVoltage: %d\n"
                "ConfiguredVoltage: %d\n"
                "MaxVoltage: %d\n"
                "Capacity: %d\n"
                "Manufacturer: %s\n"
                "PartNumber: %s\n"
                "Serial: %s\n"
                "BankName: %s\n"
                "DeviceLocator: %s\n",
                stick.GetName().c_str(),
                stick.GetCaption().c_str(),
                stick.GetDescription().c_str(),
                stick.GetSpeed(),
                stick.GetClockSpeed(),
                stick.GetMinVoltage(),
                stick.GetConfiguredVoltage(),
                stick.GetMaxVoltage(),
                stick.GetCapacity(),
                stick.GetManufacturer().c_str(),
                stick.GetPartNumber().c_str(),
                stick.GetSerial().c_str(),
                stick.GetBankName().c_str(),
                stick.GetDeviceLocator().c_str()
            );
            Shared::Windows::WinMessageBox(msg, "RAM info [Stick "+std::to_string(i)+"]");
        }
    }

    if (true) {
        auto cpu = Shared::Machine::GetCPUInfo();
        msg = Shared::SString("ProcessorName: %s\n"
            "ProcessorVendorName: %s\n"
            "ProcessorRevision: %d\n"
            "ClockSpeed: %d\n"
            "Socket: %s\n"
            "ProcessorType: %s\n"
            "ArchitectureString: %s\n"
            "Cores: %d\n"
            "Threads: %d\n",
            cpu.GetProcessorName().c_str(),
            cpu.GetProcessorVendorName().c_str(),
            cpu.GetProcessorRevision(),
            cpu.GetClockSpeed(),
            cpu.GetSocket().c_str(),
            cpu.GetProcessorTypeString().c_str(),
            cpu.GetArchitectureString().c_str(),
            cpu.GetCores(),
            cpu.GetThreads()
        );
        Shared::Windows::WinMessageBox(msg, "CPU info");
    }
    if (true) {
        auto mobo = Shared::Machine::GetMotherboardInfo();
        msg = Shared::SString("Vendor: %s\n"
            "Manufacturer: %s\n"
            "Family: %s\n"
            "ProductName: %s\n"
            "ReleaseDate: %s\n"
            "BiosVersion: %s\n"
            "Version: %s\n"
            "SKUNumber: %s\n"
            "Serial: %s",

            mobo.GetVendor().c_str(),
            mobo.GetManufacturer().c_str(),
            mobo.GetFamily().c_str(),
            mobo.GetProductName().c_str(),
            mobo.GetReleaseDate().c_str(),
            mobo.GetBiosVersion().c_str(),
            mobo.GetVersion().c_str(),
            mobo.GetSKUNumber().c_str(),
            mobo.GetSerial().c_str()
        );
        Shared::Windows::WinMessageBox(msg, "Motherboard info");
    }

    // Start localization if possible
    // not yet
    //InitLocalization(false);

    // Handle commands from the installer
    // not yet
    //HandleSpecialLaunchOptions();
    /*
    // Check MTA is launched only once
    HandleMultiLaunching();

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
