#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <Shared/sdk/SharedUtils/Path.hpp>
#include <fstream>

#ifdef CreateDirectory
#   undef CreateDirectory
#endif
#ifdef CreateFile
#   undef CreateFile
#endif
#ifdef DeleteFile
#   undef DeleteFile
#endif

namespace NightMTA::Shared::File {
    inline bool CreateDirectory(const SString& path) noexcept {
        if(Path::DirectoryExists(path))
            return true;

        std::error_code err;
        return std::filesystem::create_directory(path, err);
    }

    inline bool CreateFile(const SString& path) noexcept {
        if(Path::FileExists(path))
            return false;

        std::ofstream file(path);
        return file.good();
    }

    inline bool CreateFiles(const std::initializer_list<SString> paths) noexcept {
        for(const auto& path : paths) {
            if(!File::CreateFile(path))
                return false;
        }
        return true;
    }

    inline bool DeleteDirectory(const SString& path) noexcept {
        if(!Path::DirectoryExists(path))
            return false;
        
        std::error_code err;
        return std::filesystem::remove_all(path, err);
    }

    inline bool DeleteFile(const SString& path) noexcept {
        if(!Path::FileExists(path))
            return false;
        
        std::error_code err;
        return std::filesystem::remove(path, err);
    }

    inline std::optional<std::vector<std::uint8_t>> ReadFile(const SString& path) noexcept {
        if(!Path::FileExists(path))
            return std::nullopt;
        
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if(!file.good())
            return std::nullopt;
        file.unsetf(std::ios::skipws);
        
        std::vector<std::uint8_t> data;
        data.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        data.insert(
            data.begin(),
            std::istream_iterator<std::uint8_t>(file),
            std::istream_iterator<std::uint8_t>()
        );

        return data;
    }

    inline bool WriteFile(const SString& path, const SString& message) noexcept {
        std::ofstream file(path);
        if (!file) return false;

        file.write(message.c_str(), message.size());
        return file.good();
    }

    inline bool AppendFile(const SString& path, const SString& message) noexcept {
        std::ofstream file(path, std::ios::app);
        if (!file) return false;

        file.write(message.c_str(), message.size());
        return file.good();
    }
}
