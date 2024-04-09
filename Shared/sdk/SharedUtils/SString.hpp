#pragma once

#include <string>
#include <cstdarg>
#include <algorithm>
#include <filesystem>

#include <Shared/sdk/SharedUtils/utf8.hpp>

namespace NightMTA::SharedUtil {
    /**
     * @brief A string class that provides additional functionality like string
     * formatting and case-insensitive string comparison.
     */
    class SString : public std::string {
    public:
        /**
         * @brief Default constructor.
         *
         * Creates an empty string.
         */
        constexpr SString() noexcept : std::string() {}
        
        /**
         * @brief Constructs a string from the given C-style null-terminated string.
         * 
         * @param string The C-style null-terminated string
         */
        constexpr SString(const char* string) noexcept : std::string(string) {}

        /**
         * @brief Constructs a string from the given C-style null-terminated string
         * and the given size.
         *
         * @param string The C-style null-terminated string
         * @param size The size of the string to create
         */
        constexpr SString(const char* string, const std::size_t size) noexcept
            : std::string(string, size) {}

        
        /**
         * Constructs a string from the given std::string.
         *
         * @param string The std::string to construct the string from
         */
        constexpr SString(const std::string string) noexcept
            : std::string(string) {}
        
        /**
         * Constructs a string from the given std::wstring.
         *
         * @param string The std::wstring to construct the string from
         */
        constexpr SString(const std::wstring string) noexcept
            : std::string(UTF::UTF16ToUTF8(string)) {}

        /**
         * Constructs a string with the given size and fill character.
         *
         * @param count The size of the string to create
         * @param character The character to fill the string with
         */
        constexpr SString(const std::size_t count, const char character) noexcept
            : std::string(count, character) {}
            
        /**
         * @brief Constructs a string from the given std::filesystem::path.
         *
         * @param path The std::filesystem::path to construct the string from
         */
        SString(const std::filesystem::path path) noexcept
            : std::string(path.string()) {}
        
        /**
         * Constructs a string from the given printf-style format string and
         * variable arguments. The format string is interpreted as in the C
         * `printf` function.
         *
         * @param szFormat The format string
         * @param ... The variable arguments
         */
        explicit SString(const char* szFormat, ...) noexcept : std::string() {
            if (!szFormat) {
                return; // No format string, bail out
            }

            va_list vl;
            va_start(vl, szFormat); // Begin variable arguments list

            this->vFormat(szFormat, vl); // Format the string

            va_end(vl); // End variable arguments list
        }

        /**
         * Formats the string using the given printf-style format string and
         * variable arguments. The format string is interpreted as in the C
         * `printf` function.
         *
         * @param szFormat The format string
         * @param ... The variable arguments
         * @return A new `SString` instance with the formatted string
         */
        SString Format(const char* szFormat, ...) const noexcept {
            SString str; // Create a new string to format into
            va_list args; // Initialize variable arguments list
            va_start(args, szFormat); // Begin variable arguments list

            str.vFormat(szFormat, args); // Format the string

            va_end(args); // End variable arguments list
            return str; // Return the formatted string
        }

        /**
         * Formats the string using the given printf-style format string and
         * variable arguments. The format string is interpreted as in the C
         * `printf` function. The formatted string is stored in this string
         * instance.
         *
         * @param szFormat The format string
         * @param ... The variable arguments
         * @return A reference to this string instance
         */
        SString& Format(const char* szFormat, ...) noexcept {
            va_list args;
            va_start(args, szFormat);

            this->vFormat(szFormat, args);

            va_end(args);
            return *this;
        }

        /**
         * Formats the string using the given printf-style format string and
         * variable arguments. The format string is interpreted as in the C
         * `printf` function. The formatted string is stored in this string
         * instance.
         *
         * @param szFormat The format string
         * @param vl The variable arguments
         * @return A reference to this string instance
         */
        SString& vFormat(const char* szFormat, va_list vl) noexcept {
            // Create a copy of the variable argument list to be able to use it
            // twice (once for determining the required buffer size and once
            // for formatting the string)
            va_list vl_copy;
            va_copy(vl_copy, vl);

            // Determine the required buffer size for the formatted string
            const auto length = vsnprintf(nullptr, 0, szFormat, vl_copy);

            // End the copied variable argument list
            va_end(vl_copy);

            // If the format string was valid, allocate a buffer and format the
            // string into it
            if(length > 0) {
                char* buffer = new char[length + 1];

                vsnprintf(buffer, length + 1, szFormat, vl);

                // Assign the formatted string to this string instance and
                // delete the buffer
                this->operator=(buffer);
                delete[] buffer;
            }

            // End the original variable argument list
            va_end(vl);

            // Return a reference to this string instance
            return *this;
        }


        /**
         * Assigns the given string to this string and returns a reference
         * to this string.
         *
         * @param string The string to assign
         * @return A reference to this string
         */
        constexpr SString& operator=(SString string) noexcept {
            this->std::string::operator=(string); // Assign the given string to this string
            return *this; // Return a reference to this string
        }

        /**
         * Assigns the given C-string to this string and returns a reference
         * to this string.
         *
         * @param string The C-string to assign
         * @return A reference to this string
         */
        constexpr SString& operator=(const char* string) noexcept {
            this->std::string::operator=(string);
            return *this;
        }

        /**
         * Converts this string to a C-style null-terminated string
         * and returns a pointer to it.
         *
         * @return a pointer to a C-style null-terminated string
         */
        constexpr operator const char*() const noexcept {
            return this->c_str();
        }

        /**
         * Returns a copy of this string with all its characters
         * converted to uppercase.
         *
         * @return a new string with all its characters converted to uppercase
         */
        constexpr SString AsUpper() const noexcept {
            // Create a new string with the same size as this one
            SString temp(this->size(), 0);

            // Iterate over all characters of this string
            // and convert them to their uppercase equivalent
            std::transform(
                this->begin(), this->end(),
                temp.begin(),
                [](const auto& c) { return std::toupper(c); });

            // Return the new string
            return temp;
        }

        /**
         * Returns a copy of this string with all its characters
         * converted to lowercase.
         *
         * @return a new string with all its characters converted to lowercase
         */
        constexpr SString AsLower() const noexcept {
            // Create a new string with the same size as this one
            SString temp(this->size(), 0);

            // Iterate over all characters of this string
            // and convert them to their lowercase equivalent
            std::transform(
                this->begin(), this->end(),
                temp.begin(),
                [](const auto& c) { return std::tolower(c); });

            // Return the new string
            return temp;
        }


        /**
         * Converts all characters of this string to uppercase
         * in-place and returns a reference to this string.
         *
         * @return a reference to this string
         */
        constexpr SString& ToUpper() noexcept {
            // Convert all characters of this string to uppercase
            // and return a reference to this string
            return this->operator=(this->AsUpper());
        }

        /**
         * Converts all characters of this string to lowercase
         * in-place and returns a reference to this string.
         *
         * @return a reference to this string
         */
        constexpr SString& ToLower() noexcept {
            // Convert all characters of this string to lowercase
            // and return a reference to this string
            return this->operator=(this->AsLower());
        }

        /**
         * Checks whether this string contains the given other string.
         *
         * @param other The string to search for
         * @param ignoreCase Determines whether the search should be case-insensitive
         * @return true if this string contains the given other string, false otherwise
         */
        constexpr bool Contains(const SString& other, const bool ignoreCase = false) const noexcept {
            // Checks whether this string contains the given other string
            // and returns true if it does, false otherwise
            return ignoreCase
                ? std::as_const(*this).AsUpper().find(other.AsUpper())
                : this->find(other) != this->npos;
        }

        /**
         * Creates a new string that is a substring of this string.
         *
         * The substring starts at the given `from` index (inclusive)
         * and ends at the given `to` index (exclusive).
         * If `to` is negative, it is treated as an offset from the end of this string.
         * If `to` is not specified or is greater than the length of this string,
         * the substring extends to the end of this string.
         *
         * @param from The index to start the substring at (inclusive)
         * @param to The index to end the substring at (exclusive)
         * @return The newly created substring
         */
        constexpr SString Substring(std::int64_t from, std::int64_t to = -1) const noexcept {
            if(to < 0) to += this->size();
            if(from < 0) from += this->size();

            // Handle the case where `to` is less than `from`
            if(to > from) {
                to -= from;
            }

            return this->substr(from, to);
        }

        /**
         * Creates a new string by replacing all occurrences of the given `what`
         * string with the given `with` string.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The string to search for and replace
         * @param with The string to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return The newly created string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString Replace(
            const SString what,
            const SString with,
            bool caseInsensitive = false
        ) const noexcept {
            SString out = *this;

            // If the `what` string is the same as the `with` string,
            // there is nothing to replace, so return a copy of this string
            if(what == with)
                return out;

            // Find the first occurrence of `what` in this string
            auto index = caseInsensitive
                ? out.AsUpper().find(what.AsUpper())
                : out.find(what);

            // If `what` was not found, return a copy of this string
            if(index == out.npos)
                return out;

            // Replace all occurrences of `what` with `with`
            out.replace(index, what.length(), with);
            return out;
        }

        /**
         * Creates a new string by replacing all occurrences of the given `what`
         * string with the given `with` character.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The string to search for and replace
         * @param with The character to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return The newly created string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString Replace(
            const SString what,
            const char with,
            bool caseInsensitive = false
        ) const noexcept {
            return Replace(what, SString(1, with), caseInsensitive);
        }

        /**
         * Creates a new string by replacing all occurrences of the given `what`
         * character with the given `with` string.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The character to search for and replace
         * @param with The string to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return The newly created string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString Replace(
            const char what,
            const SString with,
            bool caseInsensitive = false
        ) const noexcept {
            return Replace(SString(1, what), with, caseInsensitive);
        }

        /**
         * Creates a new string by replacing all occurrences of the given `what`
         * character with the given `with` character.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The character to search for and replace
         * @param with The character to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return The newly created string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString Replace(
            const char what,
            const char with,
            bool caseInsensitive = false
        ) const noexcept {
            return Replace(SString(1, what), SString(1, with), caseInsensitive);
        }

        /**
         * Replaces all occurrences of the given `what` string with the given
         * `with` string.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The string to search for and replace
         * @param with The string to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return A reference to this string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString& Replace(
            const SString what,
            const SString with,
            bool caseInsensitive = false
        ) noexcept {
            return this->operator=(this->Replace(what, with, caseInsensitive));
        }
        
        /**
         * Replaces all occurrences of the given `what` string with the given
         * `with` character.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The string to search for and replace
         * @param with The character to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return A reference to this string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString& Replace(
            const SString what,
            const char with,
            bool caseInsensitive = false
        ) noexcept {
            return Replace(what, SString(1, with), caseInsensitive);
        }
        
        /**
         * Replaces all occurrences of the given `what` character with the given
         * `with` string.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The character to search for and replace
         * @param with The string to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return A reference to this string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString& Replace(
            const char what,
            const SString with,
            bool caseInsensitive = false
        ) noexcept {
            return Replace(SString(1, what), with, caseInsensitive);
        }


        /**
         * Replaces all occurrences of the given `what` character with the given
         * `with` character.
         *
         * The search is case-sensitive by default, but can be made case-insensitive
         * by setting the `caseInsensitive` parameter to true.
         *
         * @param what The character to search for and replace
         * @param with The character to replace all occurrences of `what` with
         * @param caseInsensitive Determines whether the search should be
         *        case-insensitive
         * @return A reference to this string with all occurrences of `what`
         *         replaced with `with`
         */
        constexpr SString& Replace(
            const char what,
            const char with,
            bool caseInsensitive = false
        ) noexcept {
            return Replace(SString(1, what), SString(1, with), caseInsensitive);
        }

        /**
         * Creates a new string from the given C-string.
         *
         * @param str The C-string to create the new string from
         * @param length The length of the C-string
         * @return The newly created string
         */
        friend constexpr SString operator""_ss(const char* str, size_t length) noexcept {
            return SString(str, length); // Create a new string from the given C-string
        }

        /**
         * Converts this string to a std::filesystem::path.
         *
         * This is a convenience operator that allows you to directly convert a string to a
         * std::filesystem::path without having to call the constructor of std::filesystem::path.
         *
         * @return The resulting std::filesystem::path
         */
        operator std::filesystem::path() const noexcept {
            // This string is converted to a std::filesystem::path
            return std::filesystem::path(this->c_str()); // Return the resulting path
        }
    };
}