#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#if !MTA_CPP20
#   error "Non C++20 project!"
#endif

#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::Shared::Path {
#if MTA_WIN
    constexpr char PREFERRED_SEPARATOR = '\\';
#else
    constexpr char PREFERRED_SEPARATOR = '/';
#endif

    /**
     * @brief Returns the current working directory.
     *
     * This function returns the current working directory of the system.
     * If an error occurs during the retrieval of the current working
     * directory, it returns an empty path.
     *
     * @return The current working directory of the system or an empty path
     * if an error occurred
     */
    inline std::filesystem::path GetSystemCurrentDirectory() noexcept {
        /*
         * Retrieve the current working directory of the system.
         * If an error occurs, an error code is saved into `err`.
         */
        std::error_code err;
        std::filesystem::path current_dir = std::filesystem::current_path(err);

        /*
         * If an error occurred during the retrieval of the current working
         * directory, return an empty path.
         */
        if(err)
            return std::filesystem::path();

        /*
         * Otherwise, return the retrieved current working directory.
         */
        return current_dir;
    }

    inline bool SetCurrentDirectory(const std::filesystem::path& path) noexcept {
        std::error_code err;
        std::filesystem::current_path(path, err);
        return !!err;
    }

    /**
     * @brief Checks if a file exists at the specified path.
     *
     * This function checks if a file exists at the specified path. If the
     * file does not exist or an error occurs during the check, it returns
     * `false`. Otherwise, it returns `true`.
     *
     * @param path The path to check
     * @return `true` if the file exists, `false` otherwise
     */
    inline bool FileExists(const char* path) noexcept {
        std::error_code err;
        return std::filesystem::is_regular_file(path, err);
    }
    
    /**
     * @brief Checks if a directory exists at the specified path.
     *
     * This function checks if a directory exists at the specified path.
     * If the directory does not exist or an error occurs during the check,
     * it returns `false`. Otherwise, it returns `true`.
     *
     * @param path The path to check
     * @return `true` if the directory exists, `false` otherwise
     */
    inline bool DirectoryExists(const char* path) noexcept {
        std::error_code err;
        return std::filesystem::is_directory(path, err);
    }

    /**
     * @brief Joins multiple paths together
     *
     * This function joins multiple paths together using the standard path
     * separator. The result of the join is a new path.
     *
     * @param paths The paths to join together
     * @return The joined path
     */
    inline SString PathJoin(const std::initializer_list<SString> paths) noexcept {
        std::filesystem::path result;

        // Join all the paths together using the standard path separator.
        for(const auto& path : paths) {
            result /= path;
        }

        // Return the joined path.
        return result;
    }

    /**
     * @brief Joins two paths together
     *
     * This function joins two paths together using the standard path
     * separator. The result of the join is a new path.
     *
     * @param path1 The first path
     * @param path2 The second path
     * @return The joined path
     */
    inline SString PathJoin(const SString path1, const SString path2) noexcept {
        /*
         * Join the two paths together using the standard path separator.
         */
        return PathJoin({ path1, path2 });
    }

    /**
     * @brief Replaces all forward and backward slashes in a path with the preferred
     * path separator for the current platform.
     *
     * This function replaces all forward and backward slashes in the given path
     * with the preferred path separator for the current platform. The preferred
     * path separator is defined by the `PREFERRED_SEPARATOR` constant.
     *
     * @param path The path to replace the separators in
     * @return The path with the replaced separators
     */
    inline SString PathReplaceSlash(const SString path) noexcept {
        return path.Replace("/", PREFERRED_SEPARATOR).Replace("\\", PREFERRED_SEPARATOR);
    }

    /**
     * @brief Adds a path separator to the start of a path if it doesn't already
     * have one.
     *
     * This function adds a path separator to the start of the given path if it
     * doesn't already have one. If the path already has a separator at the
     * start, the path is returned unmodified. The separator added is the
     * preferred path separator for the current platform, which is defined by
     * the `PREFERRED_SEPARATOR` constant.
     *
     * @param path The path to add a separator to
     * @return The path with a separator added to the start if necessary
     */
    inline SString PathAddSlash(const std::filesystem::path path) noexcept {
        if(path.is_absolute())
            return path;

        return SString(".") + PREFERRED_SEPARATOR + path;
    }

    /**
     * @brief Gets the relative path to a path from a root path
     *
     * This function gets the relative path from the root path to the
     * specified path. The relative path is returned as a new path.
     *
     * @param path The path to get the relative path from
     * @param root The root path to get the relative path to
     * @return The relative path
     */
    inline SString PathRelative(
        const SString path,
        const SString root = GetSystemCurrentDirectory()
    ) noexcept {
        return PathAddSlash(PathReplaceSlash(std::filesystem::relative(path, root)));
    }

    inline SString GetParentDirectory(const std::filesystem::path path) noexcept {
        return PathAddSlash(path.parent_path());
    }
#ifdef MTA_CLIENT
    inline SString GetCurrentExePath() noexcept {
        static SString exePath;
        if (!exePath.empty())
            return exePath;

        char buffer[MAX_PATH]{0};
        auto size = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        exePath = SString(buffer, size);
        return exePath;
    }
#endif
}
