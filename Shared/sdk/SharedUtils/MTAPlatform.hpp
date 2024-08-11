#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#if MTA_WIN
#   if MTA_x64
#       define MTA_OS_STRING "Windows x64"
#   else
#       define MTA_OS_STRING "Windows"
#   endif
#   define MTA_LIB_EXTENSION ".dll"
#elif MTA_LINUX
#   if MTA_x64
#       define MTA_OS_STRING "GNU/Linux x64"
#   else
#       define MTA_OS_STRING "GNU/Linux"
#   endif
#   define MTA_LIB_EXTENSION ".so"
#elif defined(__APPLE__)
#   define MTA_OS_STRING     "macOS"
#   define MTA_LIB_EXTENSION ".dylib"
#else
#   error "Unsupported operating system"
#endif

#ifdef MTA_DEBUG
#   define MTA_LIB_SUFFIX "_d"
#else
#   define MTA_LIB_SUFFIX ""
#endif
