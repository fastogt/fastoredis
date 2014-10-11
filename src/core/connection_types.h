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

    std::vector<common::string16> supportedConnectionTypes();
}

namespace common
{
    string16 convertToString16(fastoredis::connectionTypes t);
}
