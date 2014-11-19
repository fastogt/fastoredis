#pragma once

/**/

#include <vector>

#include "common/convert2string.h"

namespace fastoredis
{
    enum connectionTypes
    {
        DBUNKNOWN = 0,
        REDIS
    };

    inline connectionTypes badConnectionType()
    {
        return DBUNKNOWN;
    }

    std::vector<std::string> supportedConnectionTypes();
}

namespace common
{
    std::string convertToString(fastoredis::connectionTypes t);
}
