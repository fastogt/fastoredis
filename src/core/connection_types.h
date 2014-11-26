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

    enum ConnectionMode
    {
        /* Latency mode */
        LatencyMode,

        /* Slave mode */
        SlaveMode,

        /* Get RDB mode. */
        GetRDBMode,

        /* Pipe mode */
        PipeMode,

        /* Find big keys */
        FindBigKeysMode,

        /* Stat mode */
        StatMode,

        /* Scan mode */
        ScanMode,

        /* Intaractive mode */
        IntaractiveMode
    };
}

namespace common
{
    std::string convertToString(fastoredis::connectionTypes t);
}
