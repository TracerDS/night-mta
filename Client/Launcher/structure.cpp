#include "StdInc.hpp"
#include <Client/Loader/launching.hpp>

#define MTA_CLIENT

#include <Shared/sdk/SharedUtils/SString.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>
#include <Shared/sdk/SharedUtils/File.hpp>
#include <Shared/sdk/SharedUtils/Misc.hpp>

namespace NightMTA::Client {
    constexpr auto DIR_STRUCTURE = Shared::Misc::make_array(
        "MTA/",
        "MTA/logs/",
        "MTA/locales/"
    );
    inline const auto ExeCurrentPath = Shared::Path::GetParentDirectory(
        Shared::Path::GetCurrentExePath()
    );

    bool VerifyMTAStructure() noexcept {
        for (const auto& elem : DIR_STRUCTURE) {
            auto path = Shared::Path::PathJoin(ExeCurrentPath, elem);
            if (!Shared::Path::DirectoryExists(path))
                return false;
        }
        return true;
    }

    bool CreateMTAStructure() noexcept {
        if (VerifyMTAStructure()) return true;

        for (const auto& elem : DIR_STRUCTURE) {
            auto path = Shared::Path::PathJoin(ExeCurrentPath, elem);

            // check if directory
            if (path.EndsWith("/")) {
                if (!Shared::File::CreateDirectory(path))
                    return false;
            }

            if (!Shared::File::CreateFile(path))
                return false;
        }
        return true;
    }
}
