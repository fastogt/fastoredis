#pragma once

#include <string>

namespace common
{
    namespace net
    {
        typedef std::pair<std::string, int> hostAndPort;
        std::string toStdString(const hostAndPort &host);
        hostAndPort toHostAndPort(const std::string &host);
    }
}
