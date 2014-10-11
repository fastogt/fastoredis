#pragma once

/**/

#include <QString>

#include "common/convert2string.h"

namespace common
{
        string16 convertToString16(const QString& from);
        std::string convertToString(const QString& from);

        template<typename T>
        struct separator_traits
        {
            typedef QString type;
            typedef char16 char_type;
        #ifdef OS_WIN
            static const char_type separator = UTEXT('/');
            static const char_type* separators()
            {
                return UTEXT("//");
            }
        #elif defined OS_POSIX
            static const char_type separator = '/';
            static const char_type* separators()
            {
                return UTEXT("//");
            }
        #endif
        };

        template<>
        struct separator_traits<string16>
        {
            typedef string16 type;
            typedef type::value_type char_type;
        #ifdef OS_WIN
            static const char_type separator = UTEXT('\\');
            static const char_type* separators()
            {
                return UTEXT("\\");
            }
        #else
            static const char_type separator = '/';
            static const char_type* separators()
            {
                return UTEXT("//");
            }
        #endif
    };
}
