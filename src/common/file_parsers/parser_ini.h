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
            bool parse_ini(const std::string& path, std::map<std::string, std::string> &ref);
        }
    }
}
