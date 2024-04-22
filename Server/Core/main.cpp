#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Defines.hpp>

#include <Server/Launcher/config.hpp>

//namespace Path = NightMTA::SharedUtil::Path;
namespace Launcher = NightMTA::Server::Launcher;
using SString = NightMTA::SharedUtil::SString;

MTAEXPORT int MTAStartup(const int& argc, const char**& argv, Launcher::ServerConfig& config){
    SString bindedIP = "127.0.0.1";
    std::uint16_t bindedPort = 7777;
    std::uint16_t maxPlayers = 32;

    for(auto i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("%s %s\n", MTASA_NAME, MTASA_VERSION);
			return 0;
		}
        if(!strcmp(argv[i], "--ip")) {
			if(++i == argc) break;
			bindedIP = argv[i];
			continue;
		}
		if(!strcmp(argv[i], "--port")) {
			if(++i == argc) break;
			bindedPort = std::atoi(argv[i]);
			continue;
		}
		if(!strcmp(argv[i], "--maxplayers")) {
			if(++i == argc) break;
			maxPlayers = std::atoi(argv[i]);
			continue;
		}
    }
}