#pragma once

#include <string>
#include <cstdarg>
#include <algorithm>

namespace NightMTA::SharedUtil {
    class SString : public std::string {
    public:
        /**
         * Default constructor.
         *
         * Creates an empty string.
         */
        constexpr SString() noexcept : std::string() {}
        
        /**
         * Constructs a string from the given C-style null-terminated string.
         *
         * @param string The C-style null-terminated string
         */
        constexpr SString(const char* string) noexcept : std::string(string) {}

        constexpr SString(const char* string, const std::size_t size) noexcept : std::string(string, size) {}
        
        /**
         * Constructs a string from the given std::string.
         *
         * @param string The std::string to construct the string from
         */
        constexpr SString(const std::string string) noexcept
            : std::string(string) {}

        /**
         * Constructs a string with the given size and fill character.
         *
         * @param count The size of the string to create
         * @param character The character to fill the string with
         */
        constexpr SString(const std::size_t count, const char character) noexcept
            : std::string(count, character) {}
        
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
        SString& Format(const char* szFormat, ...)
            noexcept // Must be marked noexcept to be usable with variadic templates
        {
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

        constexpr operator const char*() const noexcept {
            return this->c_str();
        }

        /**
         * Returns a copy of this string with all its characters
         * converted to uppercase.
         *
         * @return a new string with all its characters converted to uppercase
         */
        constexpr SString ToUpper() const noexcept {
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
        constexpr SString ToLower() const noexcept {
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
            return this->operator=(std::as_const(*this).ToUpper());
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
            return this->operator=(std::as_const(*this).ToLower());
        }

        friend constexpr SString operator""_ss(const char* str, size_t length) noexcept {
            return SString(str, length);
        }
    };

}