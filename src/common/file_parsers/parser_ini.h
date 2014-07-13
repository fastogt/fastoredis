#pragma once

/**/

#include <map>

#include "common/types.h"

namespace common
{
    namespace file_parsers
    {
        namespace ini
        {
            bool parse_ini(const unicode_char* path, std::map<unicode_string, unicode_string> &ref);
        }
    }
}
