#include "common/types.h"

#include <string.h>

namespace common
{
    int32_t unicode_strcmp(const unicode_char* str1,const unicode_char* str2)
    {
        #ifdef UNICODE
            return wcscmp(str1,str2);
        #else
            return strcmp(str1,str2);
        #endif
    }

    size_t unicode_strlen(const unicode_char* str)
    {
        #ifdef UNICODE
            return wstrlen(str);
        #else
            return strlen(str);
        #endif
    }

    unicode_char* unicode_strcpy(unicode_char* destr,const unicode_char* src)
    {
        #ifdef UNICODE
            return wcscpy(destr,src);
        #else
            return strcpy(destr,src);
        #endif
    }

    ssize_t unicode_find_first_of(const unicode_char *src,const unicode_char ch)
    {
        size_t length = unicode_strlen(src);
        for(size_t n=0; n<length; n++)
        {
          if(src[n] == ch)
            return n;
        }
        return -1;
    }

    ssize_t unicode_find_last_of(const unicode_char *src,const unicode_char ch)
    {
        size_t length = unicode_strlen(src);
        for(size_t n=0; n!=length; n++)
        {
          if(src[length-1-n] == ch)
            return length-1-n;
        }
        return -1;
    }

    void unicode_strcat(unicode_char *destr,const unicode_char *src)
    {
#ifdef UNICODE
        wcscat(destr,src);
#else
        strcat(destr,src);
#endif
    }

    const unicode_char * unicode_strstr(const unicode_char *src,const unicode_char *what_search)
    {
        #ifdef UNICODE
            return wcsstr(src,what_search);
        #else
            return strstr(src,what_search);
        #endif
    }

    void unicode_perror(const unicode_char* str)
    {
        perror(str);
    }

    const unicode_char* unicode_chr(const unicode_char *string,const unicode_char ch)
    {
        while (*string && *string != ch)
            string++;

        if (*string == ch)
            return static_cast<const unicode_char *>(string);

        return NULL;
    }

    std::string escapedText(const std::string &str)
    {
        if(!str.empty() && str[str.length()-1] != '\n'){
            return str + "\n";
        }
        return str;
    }
}
