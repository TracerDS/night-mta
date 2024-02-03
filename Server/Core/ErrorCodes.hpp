#pragma once

namespace NightMTA::Server::Core {
    inline constexpr auto ERROR_NO_ERROR = 0;
    inline constexpr auto ERROR_NO_NETWORK_LIBRARY = 1;
    inline constexpr auto ERROR_NETWORK_LIBRARY_FAILED = 2;
    inline constexpr auto ERROR_LOADING_MOD = 3;
    inline constexpr auto ERROR_OTHER = 4;
}