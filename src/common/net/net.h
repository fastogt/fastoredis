#pragma once

/**/

#include "common/convert2string.h"

namespace common
{
    namespace net
    {
        typedef std::pair<std::string, uint16_t> hostAndPort;
        bool getRemoteMacAddress(const std::string &host, std::string &out_mac_address);
    }

    std::string convertToString(const net::hostAndPort& from);
}
