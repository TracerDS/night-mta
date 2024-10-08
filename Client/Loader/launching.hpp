#pragma once

namespace NightMTA::Client {
    void HandleMultiLaunching() noexcept;

    bool VerifyMTAStructure() noexcept;
    bool CreateMTAStructure() noexcept;
}
