#pragma once

#include <Shared/sdk/SharedUtils/Path.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::Client {
    inline bool VerifyMTAStructure() noexcept {
        using Shared::SString;
        namespace Path = Shared::Path;
        
        static const SString mtaPath = Path::PathJoin(Path::GetSystemCurrentDirectory(), "MTA");
        if (!Path::DirectoryExists(mtaPath))
            return false;
        
        return true;
    }
}
