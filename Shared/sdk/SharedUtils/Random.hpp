#pragma once

#include <random>

namespace NightMTA::Shared::Random {
    inline std::mt19937 GetRandomEngine() noexcept {
        static std::mt19937 engine;
        static bool engineInitialized = false;
        if (engineInitialized)
            return engine;

        std::random_device rd;
        engine = std::mt19937(rd());
        engineInitialized = true;
        return engine;
    }

    // inclusive
    template <std::integral T>
    T RandomInRange(const T& min, const T& max) noexcept {
        std::uniform_int_distribution<T> dist(min, max);
        static auto engine = GetRandomEngine();
        return dist(engine);
    }
    
    template <std::floating_point T>
    T RandomInRange(const T& min, const T& max) noexcept {
        std::uniform_real_distribution<T> dist(min, max);
        static auto engine = GetRandomEngine();
        return dist(engine);
    }
}
