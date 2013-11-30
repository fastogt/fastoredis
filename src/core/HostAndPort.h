#pragma once

#include <string>
#include <vector>

namespace fastoredis
{
    typedef std::pair<std::string, unsigned> hostAndPort;
    namespace detail
    {
        std::string toStdString(const hostAndPort &host);
        hostAndPort toHostAndPort(const std::string &host);
    }
}
