#pragma once

#ifdef __cplusplus
#   define MTA_CPP 1
#   if __cplusplus >= 202302
#       define MTA_CPP23 1
#   else
#       define MTA_CPP23 0
#   endif
#   if __cplusplus >= 202002L
#       define MTA_CPP20 1
#   else
#       define MTA_CPP20 0
#   endif
#   if __cplusplus >= 201703L
#       define MTA_CPP17 1
#   else
#       define MTA_CPP17 0
#   endif
#   if __cplusplus >= 201402L
#       define MTA_CPP14 1
#   else
#       define MTA_CPP14 0
#   endif
#   if __cplusplus >= 201103L
#       define MTA_CPP11 1
#   else
#       define MTA_CPP11 0
#   endif
#   if __cplusplus >= 199711L
#       define MTA_CPP97 1
#   else
#       define MTA_CPP97 0
#   endif
#else
#   define MTA_CPP 0
#endif

#if defined(__clang__)
#   define MTA_CLANG 1
#   define MTA_GNU 0
#   define MTA_MSVC 0
#elif defined(__GNUC__)
#   define MTA_CLANG 0
#   define MTA_GNU 1
#   define MTA_MSVC 0
#elif defined(_MSC_VER)
#   define MTA_CLANG 0
#   define MTA_GNU 0
#   define MTA_MSVC 1
#endif

//
// Architecture
//
#if defined(_WIN32)
#   define MTA_WIN 1
#   define MTA_LINUX 0
#else
#   define MTA_WIN 0
#   define MTA_LINUX 1
#endif

#if defined(__x86_64__) || defined(_WIN64)
#   define MTA_x64 1
#   define MTA_x86 0
#else
#   define MTA_x64 0
#   define MTA_x86 1
#endif

//
// Location of server binaries
//
#if MTA_x86
    #define SERVER_BIN_PATH     ""
    #define SERVER_BIN_PATH_MOD "mods/deathmatch/"
#else
    #define SERVER_BIN_PATH     "x64/"
    #define SERVER_BIN_PATH_MOD "x64/"
#endif

//
// Location of server crash dumps
//
#if MTA_WIN
    #define SERVER_DUMP_PATH "dumps"
#else
    #define SERVER_DUMP_PATH "mods/deathmatch/dumps"
#endif

#if MTA_WIN
#   define MTAEXPORT extern "C" __declspec(dllexport)
#else
#   define MTAEXPORT extern "C" __attribute__ ((visibility ("default")))
#endif