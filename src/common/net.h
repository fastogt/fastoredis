#pragma once

/**/

#include "common/convert2string.h"

namespace common
{
    namespace net
    {
        typedef std::pair<common::string16, uint16_t> hostAndPort;
        bool getRemoteMacAddress(const common::string16 &host, std::string &out_mac_address);
    }

    common::string16 convertToString16(const net::hostAndPort& from);
}
