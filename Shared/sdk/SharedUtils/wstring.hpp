#pragma once

#include <string>

namespace NightMTA::SharedUtil {
    class WString : public std::wstring {
    public:
        operator const wchar_t*() const { return c_str(); }
    };
}