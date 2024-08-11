#include "StdInc.hpp"

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/UTF8.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>
#include <Shared/sdk/SharedUtils/EventLog.hpp>
#include <Shared/sdk/SharedUtils/Time.hpp>

#include <sdk/dirStructure.hpp>

namespace Shared = NightMTA::Shared;
namespace Windows = Shared::Windows;
namespace Path = Shared::Path;

using SString = Shared::SString;

/**
 * Main entry point for the Windows application.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application. This parameter is always NULL.
 * @param lpCmdLine The command line for the application, excluding the program name.
 * @param nShowCmd Controls how the window is to be shown.
 *
 * @return An integer that indicates the exit status of the application.
 *
 * @throws None
 */
int WINAPI WinMain (
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    // Check if the current Windows version is at least Windows 10.
    // If it's not, display an error message and exit the application.
    if (!Windows::WindowsVersion::IsWin10OrGreater()) {
        Windows::WinMessageBox("Unsupported Windows version.", MB_ICONERROR);
        return 1;
    }

    // Set the application model ID for the application.
    // This is used for Windows 10 specific features like the Start menu and notifications.
    // The ID is set to the name of the application defined in the Version namespace.
    // The Utils::UTF8::FromUTF8 function is used to convert the name from UTF-8 to UTF-16,
    // which is the format required by the Windows API.
    Windows::SetAppID(Shared::UTF8::FromUTF8(Shared::Version::MTA_NAME));

    // Set the error mode to SEM_FAILCRITICALERRORS, which prevents Windows from displaying the Windows Error Reporting dialog box.
    // This is done to prevent the user from accidentally sending a bug report to Microsoft, which could potentially contain sensitive information.
    // The error mode is set using the SetErrorMode function from the Windows API.
    // The SEM_FAILCRITICALERRORS flag is passed as the argument to the function, which disables the error reporting dialog box.
    // By setting the error mode to SEM_FAILCRITICALERRORS, critical errors will still be logged to the Windows event log, but the user will not be prompted to send a bug report.
    SetErrorMode(SEM_FAILCRITICALERRORS);

    if (!NightMTA::Client::VerifyMTAStructure()) {
        Windows::WinMessageBox("MTA structure is corrupted! Please reinstall.", MB_ICONERROR);
        return 1;
    }

    SString loaderFilename = "loader" MTA_LIB_SUFFIX ".dll";

    SString mtaRootPath = Path::GetParentDirectory(Path::GetCurrentExePath());
    SString mtaPath = Path::PathJoin(mtaRootPath, "MTA");
    SString loaderPath = Path::PathJoin(mtaPath, loaderFilename);

    Path::SetCurrentDirectory(mtaRootPath);

    Shared::Misc::DynamicLibrary library;
    if (!library.Load(loaderPath)) {
        Windows::WinMessageBox("Cannot find " + loaderPath.Quoted(), MB_ICONERROR);
        return 1;
    }

    auto DoWinMain = Shared::Misc::GetProcAddress<int(*)(HINSTANCE, HINSTANCE, LPSTR, int)>(
        library, "DoWinMain"
    );

    if (!DoWinMain) {
        Windows::WinMessageBox(loaderFilename.Quoted() + " is corrupted", MB_ICONERROR);
        return 1;
    }
    return DoWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
