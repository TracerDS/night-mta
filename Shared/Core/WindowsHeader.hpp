#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if MTA_WIN
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#endif