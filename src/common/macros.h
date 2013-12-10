#pragma once

#include <stdio.h>
#include <iostream>
#include "common/common_config.h"
#include <cassert>
#include <sys/types.h>
#define VERIFY(x) assert(x)
#ifdef NDEBUG
#define DCHECK(x)
#else
#define DCHECK(x) assert(x)
#endif
#ifdef UNICODE
#define UTEXT(quote) L##quote
typedef wchar_t unicode_char;
#else
#define UTEXT(quote) quote  
typedef char unicode_char;
#endif // !UNICODE
#define STRINGIZE_HELPER(x)				#x
#define STRINGIZE(x)					STRINGIZE_HELPER(x)

typedef std::basic_string<unicode_char> unicode_string;
typedef std::basic_ostream<unicode_char> unicode_ostream;
typedef std::basic_istream<unicode_char> unicode_istream;
typedef std::basic_ofstream<unicode_char> unicode_ofstream;
typedef std::basic_ifstream<unicode_char> unicode_ifstream;

namespace common
{
    enum tribool
    {
        FAIL=0,
        SUCCESS=1,
        INDETERMINATE=-1
    };
    int unicode_strcmp(const unicode_char* str1,const unicode_char* str2);
    size_t unicode_strlen(const unicode_char* str);
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
    template<typename... targs_t>
    size_t unicode_strlen(const unicode_char* str,targs_t... args)
    {
        return unicode_strlen(str)+unicode_strlen(args...);
    }
    #endif
    unicode_char* unicode_strcpy(unicode_char* destr,const unicode_char* src);
    ssize_t unicode_find_first_of(const unicode_char *src,const unicode_char ch);
    ssize_t unicode_find_last_of(const unicode_char *src,const unicode_char ch);
    void unicode_strcat(unicode_char *destr,const unicode_char *src);
    const unicode_char * unicode_strstr(const unicode_char *src,const unicode_char *what_search);
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
    template<typename... Args>
    int inline unicode_sprintf(unicode_char *ref,const unicode_char* format,Args... args)
    {
        return sprintf (ref, format, args...);
    }
    #else
    template<typename A1>
    int inline unicode_sprintf(unicode_char *ref,const unicode_char* format,A1 a1)
    {
        return sprintf(ref, format, a1);
    }
    template<typename A1, typename A2>
    int inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2)
    {
        return sprintf(ref, format, a1, a2);
    }
    template<typename A1, typename A2, typename A3>
    int inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2, A3 a3)
    {
        return sprintf(ref, format, a1, a2, a3);
    }
    template<typename A1, typename A2, typename A3, typename A4>
    int inline unicode_sprintf(unicode_char *ref,const unicode_char* format, A1 a1, A2 a2, A3 a3, A4 a4)
    {
        return sprintf(ref, format, a1, a2, a3, a4);
    }
    #endif
    void unicode_perror(const unicode_char* str);
    const unicode_char* unicode_chr(const unicode_char *string,const unicode_char ch);
}
