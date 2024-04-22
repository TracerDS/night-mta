#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/sdk/SharedUtils/MTAPlatform.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>
#include <Shared/Core/DynamicLibrary.def.hpp>

#include <Server/Launcher/config.hpp>

#define LIB_CORE_PATH SERVER_BIN_PATH "core" MTA_LIB_SUFFIX MTA_LIB_EXTENSION

namespace Path = NightMTA::SharedUtil::Path;
namespace Launcher = NightMTA::Server::Launcher;

using SString = NightMTA::SharedUtil::SString;
using MTAStartupFunc = int(*)(const int& argc, const char**& argv);

int main(int argc, const char* argv[]) {
#if MTA_WIN
	if (!NightMTA::SharedUtil::Windows::IsWin10OrGreater()){
		MessageBoxA(nullptr, "Windows 10 or newer is required to run " MTASA_NAME "!",
			MTASA_NAME, MB_HELP | MB_ICONSTOP
		);
		return 1;
	}
#endif

	Launcher::ServerConfig config;
	
	for(auto i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printf(
				"Usage: %s [OPTION]\n\n"
				"  -h, --help           Shows this help\n"
				"  -v, --version        Shows the program version\n"
				"  --ip [ADDR]          Set IP address\n"
				"  --port [PORT]        Set port\n"
				"  --maxplayers [max]   Set maxplayers\n",
				Path::PathRelative(argv[0], Path::GetSystemCurrentDirectory()).c_str());
			return 0;
		}
	}

	const auto status = [&argc, &argv, &config]() -> int {
		NightMTA::Shared::Core::DynamicLibrary lib;
		if(!lib.Load(LIB_CORE_PATH)){
	#if MTA_WIN
			NightMTA::Shared::WinMessage msg(GetLastError());

			// Display the error message and exit the process
			fprintf(stderr, "Loading library %s failed (%s)\n", "core", msg.c_str());
	#else
			const char* szError = dlerror();
			fprintf(stderr, "Loading library %s failed", "core");
			if (szError)
				fprintf(stderr, " (%s)\n", szError);
	#endif
			return 1;
		}

		auto addr = reinterpret_cast<MTAStartupFunc>(lib.GetProcAddr("MTAStartup"));
		if(!addr) {
	#if MTA_WIN
			NightMTA::Shared::WinMessage msg(GetLastError());
			fprintf(stderr, "Loading \"MTAStartup\" function failed (%s)\n", msg.c_str());
	#else		
			fprintf(stderr, "Loading \"MTAStartup\" function failed (%s)\n", dlerror());
	#endif
			return 1;
		}
		return addr(argc, argv);
	}();

	printf("Press enter to continue...\n");
	std::cin.get();
	return status;
}
