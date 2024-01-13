#pragma once

#include <optional>
#include <string_view>

namespace bencode {

consteval bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

// function to convert string to int at compile time
consteval int compile_stoi(std::string_view str, size_t index=0, int value=0)
{
    if (str[index] == 'i')
    {
        // skip 'i'  and parse until 'e'
        return compile_stoi(str, index + 1, 0);
    }
    if (str[index] == '-')
    {
        // make sure to make this number negative
        return -1 * compile_stoi(str, index + 1, 0);
    }
    if (str[index] == 'e')
    {
        // end of int
        return value;
    }
    return compile_stoi(str, index + 1, value * 10 + (str[index] - '0'));
}

// TODO: Implement int parsing for the bencode fromat
// Bencode encoded integers have the format `i<number>e`. Your function should return the number.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the integer number,
// which might be negative.
//
// We assume that the passed string_view contains only one encoding, and the last character
// of a valid encoding is 'e'. Therefore, you do not need to handle cases like 'i4ei42e'.
//
// Example of a valid encoding: "i42e", which should be parsed to the integer 42.
// Example of a valid encoding: "i-42e", which should be parsed to the integer -42.
//
// Edges cases you might need to look out for:
// - String not starting with 'i', or ending with 'e'
// - Handle empty string
// - Handle if a non-digit number is between 'i' and 'e'
consteval std::optional<int> parse_int(std::string_view str) {
    if (str.empty())
    {
        return std::nullopt;
    }
    if (str.front() != 'i')
    {
        return std::nullopt;
    }
    if (str.back() != 'e')
    {
        return std::nullopt;
    }
    if (str.size() == 2)
    {
        return std::nullopt;
    }
    for (size_t i{1}; i < str.size() - 1; ++i)
    {
        if (!is_digit(str[i]) && i != 1)
        {
            return std::nullopt;
        }
    }
    return compile_stoi(str);
}

// TODO: Implement byte string parsing for the bencode fromat
// Bencode byte strings have the format <length>:<string>, where `length` is a number.
// The colon ':' separates the length and the actual string.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the string.
// For failure, the optional should be empty.
//
// Example of a valid byte string: "5:hello", where the length is 5, and the string is "hello".
//
// You may assume that all characters in the byte string are ASCII characters.
//
// We do not check for all edge cases, important edgecases your implementation should handle:
// - The byte string doesn't start with a number
// - The byte string doesn't have a colon
// - It is fine for the length to be shorter than the string, just return the string according to the length
// - It is NOT valid for the string to be shorter than the specified length
// - The string may contain colons
consteval std::optional<std::string_view> parse_byte_string(std::string_view str) {
    // check if the value until the first colon is a number
    bool contains_colon_correct{false};
    for (size_t i{0}; i < str.size(); ++i)
    {
        if (str[i] == ':')
        {
            contains_colon_correct = true;
            break;
        }
        if (!is_digit(str[i]))
        {
            // found non-digit before colon
            return std::nullopt;
        }
    }

    // did not contain the colon in the correct spot or did not have a colon at all
    if (contains_colon_correct)
    {

        // obtain length information 
        size_t colon_pos = str.find(':');
        size_t length = 0;
        for (char digit : str.substr(0, colon_pos)) {
            length = length * 10 + (digit - '0');
        }

        if (str.size() - str.find(':') - 1 < length) // counting from 0 hence why we minus an additional 1
        {
            return std::nullopt;
        }
        // return string according to length
        return str.substr(colon_pos + 1, length);
    }

    return std::nullopt;
}
} // namespace bencode
