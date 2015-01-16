#pragma once

#include <iosfwd>

#include "common/string16.h"

#ifdef BOOST_ENABLED
#include <boost/atomic.hpp>
#elif defined(HAVE_CXX_STANDART)
#include <atomic>
#endif

enum tribool
{
    FAIL = 0,
    SUCCESS = 1,
    INDETERMINATE = -1
};

namespace common
{
#ifdef BOOST_ENABLED
    typedef boost::atomic_uchar atomic_uchar_t;
#elif defined(HAVE_CXX_STANDART)
    typedef std::atomic_uchar atomic_uchar_t;
#endif

    typedef uint8_t byte_t;
    typedef std::basic_string<byte_t> buffer_type;

    typedef std::basic_ostream<char16, string16_char_traits> string16_ostream;
    typedef std::basic_istream<char16, string16_char_traits> string16_istream;
    typedef std::basic_ofstream<char16, string16_char_traits> string16_ofstream;
    typedef std::basic_ifstream<char16, string16_char_traits> string16_ifstream;

    std::string escapedText(const std::string &str);
    std::string doubleEscapedText(std::string str);

    typedef int64_t time64_t;
}
