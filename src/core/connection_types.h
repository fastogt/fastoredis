#pragma once

#include <vector>
#include <string>

namespace fastoredis
{
    enum connectionTypes
    {
        DBUNKNOWN = 0,
        REDIS
    };

    static inline connectionTypes badConnectionType()
    {
        return DBUNKNOWN;
    }
    std::vector<std::string> supportedConnectionTypes();
    std::string toStdString(connectionTypes t);
    connectionTypes toConnectionType(const std::string &text);
}
