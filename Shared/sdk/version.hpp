#pragma once

#define MTASA_VERSION_MAJOR         1
#define MTASA_VERSION_MINOR         6
#define MTASA_VERSION_MAINTENANCE   0
#define MTASA_VERSION_TYPE          VERSION_TYPE_CUSTOM
#define MTASA_VERSION_BUILD         0

#define VERSION_TYPE_CUSTOM         0x1
#define VERSION_TYPE_EXPERIMENTAL   0x3
#define VERSION_TYPE_UNSTABLE       0x5
#define VERSION_TYPE_UNTESTED       0x7
#define VERSION_TYPE_RELEASE        0x9

#ifdef MTA_CLIENT
#   define MTA_DM_FULL_STRING "MTA:SA Client"
#else
#   define MTA_DM_FULL_STRING "MTA:SA Server"
#endif

#define QUOTE_DEFINE2(c) #c
#define QUOTE_DEFINE(x) QUOTE_DEFINE2(x)

#define MTA_DM_VERSION ( \
    (MTASA_VERSION_MAJOR) << 8 | \
    (MTASA_VERSION_MINOR) << 4 | \
    (MTASA_VERSION_MAINTENANCE) << 0)
#if MTASA_VERSION_MAINTENANCE == 0
#   define MTA_DM_VERSIONSTRING QUOTE_DEFINE(MTASA_VERSION_MAJOR) "." \
    QUOTE_DEFINE (MTASA_VERSION_MINOR)
#else
#   define MTA_DM_VERSIONSTRING QUOTE_DEFINE(MTASA_VERSION_MAJOR) "." \
    QUOTE_DEFINE (MTASA_VERSION_MINOR) "." QUOTE_DEFINE (MTASA_VERSION_MAINTENANCE)
#endif

#if MTASA_VERSION_TYPE == VERSION_TYPE_CUSTOM
    #define MTA_DM_BUILDTYPE        "custom"
    #define MTA_DM_BUILDTAG_SHORT   MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE
    #define MTA_DM_BUILDTAG_LONG    MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE
#elif MTASA_VERSION_TYPE == VERSION_TYPE_UNSTABLE
    #define MTA_DM_BUILDTYPE        "unstable"
    #define MTA_DM_BUILDTAG_SHORT   MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE \
        "-" QUOTE_DEFINE ( MTASA_VERSION_BUILD )
    #define MTA_DM_BUILDTAG_LONG    MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE \
        "-" QUOTE_DEFINE ( MTASA_VERSION_BUILD )
#elif MTASA_VERSION_TYPE == VERSION_TYPE_UNTESTED
    #define MTA_DM_BUILDTYPE        "untested"
    #define MTA_DM_BUILDTAG_SHORT   MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE \
        "-" QUOTE_DEFINE ( MTASA_VERSION_BUILD )
    #define MTA_DM_BUILDTAG_LONG    MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE \
        "-" QUOTE_DEFINE ( MTASA_VERSION_BUILD )
    #define MTA_DM_PUBLIC_CONNECTIONS
    #define MTA_DM_EXPIRE_DAYS      60
#elif MTASA_VERSION_TYPE == VERSION_TYPE_RELEASE
    #define MTA_DM_BUILDTYPE        "release"
    #define MTA_DM_BUILDTAG_SHORT   MTA_DM_VERSIONSTRING
    #define MTA_DM_BUILDTAG_LONG    MTA_DM_VERSIONSTRING "-" MTA_DM_BUILDTYPE \
        "-" QUOTE_DEFINE ( MTASA_VERSION_BUILD )
    #define MTA_DM_PUBLIC_CONNECTIONS
#else
    #error "Incorrect MTASA_VERSION_TYPE"
#endif