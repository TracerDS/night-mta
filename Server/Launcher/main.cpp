#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>

#define LIB_CORE SERVER_BIN_PATH "core" MTA_LIB_SUFFIX MTA_LIB_EXTENSION

namespace Path = NightMTA::SharedUtil::Path;

namespace fs = std::filesystem;
/*
int mainFunc(int argc, const char* argv[]) {
	std::string strLaunchDirectory = argv[0];
	auto offset = strLaunchDirectory.find_last_of("/\\");
	if (offset != strLaunchDirectory.npos) {
		strLaunchDirectory = strLaunchDirectory.substr(0, offset);
	}
#if MTA_WIN
	if (!NightMTA::SharedUtil::Windows::IsWindows8OrGreater()) {
		printf("This version of MTA requires Windows 8 or later\n");
		return 1;
	}
#endif

	std::ifstream file(LIB_CORE);
	if (!file) {
		try {
			fs::current_path(strLaunchDirectory);
		} catch (const std::exception& exc) {
			printf(
				"ERROR: Cannot set working directory!\n"
				"* Error message: \"%s\"\n"
				, exc.what()
			);
			return 1;
		}
	}

	NightMTA::Shared::Core::DynamicLibrary coreLib;
	if (!coreLib.Load(LIB_CORE)) {
		printf(
			"ERROR: Could not load %s\n"
			"* Check installed data files.\n"
#if MTA_WIN
			"* Check installed Microsoft Visual C++ Redistributable Package (x86).\n"
#endif
			, LIB_CORE);
		return 1;
	}

	// Grab the entrypoint
	using Main_t = int(int, const char* []);

	Main_t* pfnEntryPoint = reinterpret_cast<Main_t*>(coreLib.GetProcAddr("Run"));
	if (!pfnEntryPoint) {
		printf("ERROR: Bad file: %s\n", LIB_CORE);
		return 1;
	}
	// Call it and return what it returns
	return pfnEntryPoint(argc, argv);
}
*/
int main(int argc, const char* argv[]) {
#if MTA_WIN
	if (!NightMTA::SharedUtil::Windows::IsWin10OrGreater()){
		MessageBoxA(nullptr, "Windows 10 or newer is required to run " MTASA_NAME "!",
			MTASA_NAME, MB_HELP | MB_ICONSTOP
		);
		return 1;
	}
#endif

	for(auto i = 1; i < argc; i++) {
		if (
#if MTA_WIN
			strcmp(argv[1], "/?") == 0 ||
#endif
			strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0
		) {
			printf(
				"Usage: %s [OPTION]\n\n"
				"  -v                   Shows the program version\n"
				"  -s                   Run server in silent mode\n"
#if !MTA_WIN
				"  -d                   Run server daemonized\n"
#endif
				"  -t                   Run server with a simple console\n"
				"  -f                   Run server with a standard console (Default)\n"
				"  -n                   Disable the usage of ncurses (For screenlog)\n"
				"  -u                   Disable output buffering and flush instantly (useful for screenlog)\n"
#if !MTA_WIN
				"  -x                   Disable simplified crash reports (To allow core dumps)\n"
				"  --child-process      Run server without output buffering and with a readyness event\n"
#endif
				"  -D [PATH]            Use as base directory\n"
				"  --config [FILE]      Alternate mtaserver.conf file\n"
				"  --ip [ADDR]          Set IP address\n"
				"  --port [PORT]        Set port\n"
				"  --httpport [PORT]    Set http port\n"
				"  --maxplayers [max]   Set maxplayers\n"
				"  --novoice            Disable voice communication\n"
			, Path::PathRelative(argv[0], Path::GetSystemCurrentDirectory()).c_str());
			return 1;
		}
    }

	/*
	mainFunc(argc, argv);

	// Wait for a key then exit
	printf("Press enter to continue...\n");
	std::cin.get();
	return 0;
	*/
}