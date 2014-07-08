#pragma once

/**/

#include "common/convert2string.h"

namespace common
{
    namespace net
    {
        typedef std::pair<unicode_string, uint16_type> hostAndPort;
        bool getRemoteMacAddress(const unicode_string &host, unicode_string &out_mac_address);
    }

    unicode_string convert2string(const net::hostAndPort& from);
}
