#pragma once

/**/

#include <stdint.h>

#include <stdio.h>
#include <iosfwd>
#include <string>

#include "common/macros.h"

#ifdef UNICODE
    #define UTEXT(quote) L##quote
#else
    #define UTEXT(quote) quote
#endif // !UNICODE

#ifdef UNICODE
    typedef wchar_t unicode_char;
#else
    typedef char unicode_char;
#endif // !UNICODE

    typedef uint8_t byte_type;
    typedef std::basic_string<byte_type> buffer_type;

    typedef std::basic_string<unicode_char> unicode_string;
    typedef std::basic_ostream<unicode_char> unicode_ostream;
    typedef std::basic_istream<unicode_char> unicode_istream;
    typedef std::basic_ofstream<unicode_char> unicode_ofstream;
    typedef std::basic_ifstream<unicode_char> unicode_ifstream;

    enum tribool
    {
        FAIL=0,
        SUCCESS=1,
        INDETERMINATE=-1
    };

namespace common
{
    int32_t unicode_strcmp(const unicode_char* str1,const unicode_char* str2);
    size_t unicode_strlen(const unicode_char* str);
    unicode_char* unicode_strcpy(unicode_char* destr,const unicode_char* src);
    ssize_t unicode_find_first_of(const unicode_char *src,const unicode_char ch);
    ssize_t unicode_find_last_of(const unicode_char *src,const unicode_char ch);
    void unicode_strcat(unicode_char *destr,const unicode_char *src);
    const unicode_char * unicode_strstr(const unicode_char *src,const unicode_char *what_search);
    void unicode_perror(const unicode_char* str);
    const unicode_char* unicode_chr(const unicode_char *string, const unicode_char ch);

    template<typename T>
    inline T normalize(T u)
    {
        return u;
    }

    inline const unicode_char* normalize(unicode_string u)
    {
        return u.c_str();
    }

    template<typename A1>
    int32_t inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1)
    {
        return sprintf(ref, format, normalize(a1));
    }

    template<typename A1, typename A2>
    int32_t inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2)
    {
        return sprintf(ref, format, normalize(a1), normalize(a2));
    }

    template<typename A1, typename A2, typename A3>
    int32_t inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2, A3 a3)
    {
        return sprintf(ref, format, normalize(a1), normalize(a2), normalize(a3));
    }

    template<typename A1, typename A2, typename A3, typename A4>
    int32_t inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2, A3 a3, A4 a4)
    {
        return sprintf(ref, format, normalize(a1), normalize(a2), normalize(a3), normalize(a4));
    }

    std::string escapedText(const std::string &str);
}
