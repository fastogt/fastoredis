#pragma once

#include <iosfwd>

#include "common/string16.h"

enum tribool
{
    FAIL = 0,
    SUCCESS = 1,
    INDETERMINATE = -1
};

namespace common
{
    typedef uint8_t byte_type;

    typedef std::basic_string<byte_type> buffer_type;

    typedef std::basic_ostream<char16, string16_char_traits> string16_ostream;
    typedef std::basic_istream<char16, string16_char_traits> string16_istream;
    typedef std::basic_ofstream<char16, string16_char_traits> string16_ofstream;
    typedef std::basic_ifstream<char16, string16_char_traits> string16_ifstream;

    std::string escapedText(const std::string &str);
    std::string doubleEscapedText(std::string str);
}
