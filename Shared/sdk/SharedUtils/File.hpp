#pragma once

#include <filesystem>

namespace NightMTA::SharedUtil::File { 
    inline std::filesystem::path GetSystemCurrentDirectory() noexcept {
        std::error_code err;
        return std::filesystem::current_path(err);
    }
    
    inline bool FileExists(const char* path) noexcept {
        std::error_code err;
        return std::filesystem::is_regular_file(path, err);
    }
    
    inline bool DirectoryExists(const char* path) noexcept {
        std::error_code err;
        return std::filesystem::is_directory(path, err);
    }
}