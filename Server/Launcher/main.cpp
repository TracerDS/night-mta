#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <Shared/Core/DynamicLibrary.hpp>
#include <Shared/Core/WindowsMessages.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>

#define LIB_CORE SERVER_BIN_PATH "core" MTA_LIB_SUFFIX MTA_LIB_EXTENSION

namespace fs = std::filesystem;

int mainFunc(int argc, const char* argv[]) {
	std::string strLaunchDirectory = argv[0];
	auto offset = strLaunchDirectory.find_last_of("/\\");
	if (offset != strLaunchDirectory.npos) {
		strLaunchDirectory = strLaunchDirectory.substr(0, offset);
	}
#ifdef _WIN32
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
#ifdef _WIN32
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

int main(int argc, const char* argv[]) {
	for(auto i = 1; i < argc; i++) {
		if (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
			printf(
				"Usage: %s [OPTION]\n\n"
				"  -v                   Shows the program version\n"
				"  -s                   Run server in silent mode\n"
#ifndef _WIN32
				"  -d                   Run server daemonized\n"
#endif
				"  -t                   Run server with a simple console\n"
				"  -f                   Run server with a standard console (Default)\n"
				"  -n                   Disable the usage of ncurses (For screenlog)\n"
				"  -u                   Disable output buffering and flush instantly (useful for screenlog)\n"
#ifndef _WIN32
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
			, "");
			return 1;
		}
    }

	mainFunc(argc, argv);

	// Wait for a key then exit
	printf("Press enter to continue...\n");
	std::cin.get();
	return 0;
}