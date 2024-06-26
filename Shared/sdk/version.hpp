#pragma once

#define MTASA_NAME "NightMTA"

#define MTASA_VERSION_MAJOR         1
#define MTASA_VERSION_MINOR         6
#define MTASA_VERSION_BUILD         0
#define MTASA_VERSION_TYPE          VERSION_TYPE_CUSTOM

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

#define MTASA_VERSION \
    QUOTE_DEFINE(MTASA_VERSION_MAJOR) "." \
    QUOTE_DEFINE(MTASA_VERSION_MINOR) "." \
    QUOTE_DEFINE(MTASA_VERSION_BUILD)