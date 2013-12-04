#pragma once

#include "global/global.h"

namespace fastoredis
{
    enum connectionTypes
    {
        DBUNKNOWN = 0,
        REDIS
    };

    namespace detail
    {
        static inline connectionTypes badConnectionType()
        {
            return DBUNKNOWN;
        }
        std::vector<std::string> supportedConnectionTypes();
        std::string toStdString(connectionTypes t);
        connectionTypes toConnectionType(const std::string &text);
    }
}
