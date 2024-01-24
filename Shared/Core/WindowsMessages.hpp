#pragma once

#include "WindowsHeader.hpp"

namespace NightMTA::Shared {
#ifdef _WIN32
    class WinMessage {
        static constexpr DWORD m_dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        static constexpr DWORD m_dwLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

        char* m_lpBuffer;
    public:
        WinMessage(const DWORD error) noexcept {
            FormatMessage(this->m_dwFlags, nullptr, error, this->m_dwLangID,
                this->m_lpBuffer, 0, nullptr);
        }
        ~WinMessage() noexcept {
            this->Free();
        }

        constexpr const char* c_str() const noexcept { return m_lpBuffer; }
        constexpr const char* operator()() const noexcept { return this->c_str(); }

        void Free() noexcept {
            if (!this->m_lpBuffer)
                return;

            LocalFree(this->m_lpBuffer);
            this->m_lpBuffer = nullptr;
        }
    };

#endif
}