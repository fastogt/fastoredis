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

    static inline connectionTypes badConnectionType()
    {
        return DBUNKNOWN;
    }

    std::vector<std::string> supportedConnectionTypes();
}

namespace common
{
    unicode_string convert2string(fastoredis::connectionTypes t);
}
