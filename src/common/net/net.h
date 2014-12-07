#pragma once

/**/

#include "common/convert2string.h"

namespace common
{
    namespace net
    {
        typedef std::pair<std::string, uint16_t> hostAndPort;
    }

    std::string convertToString(const net::hostAndPort& from);
}
