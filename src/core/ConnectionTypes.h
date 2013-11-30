#pragma once

#include <vector>
#include <string>

namespace fastoredis
{
    enum connectionTypes
    {
        UNKNOWN = 0,
        REDIS
    };

    namespace detail
    {
        static inline connectionTypes badConnectionType()
        {
            return UNKNOWN;
        }
        std::vector<std::string> supportedConnectionTypes();
        std::string toStdString(connectionTypes t);
        connectionTypes toConnectionType(const std::string &text);
    }
}
