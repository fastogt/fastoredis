#pragma once

#include "common/convert2string.h"

namespace fastoredis
{
    enum connectionTypes
    {
        DBUNKNOWN = 0,
        REDIS,
        MEMCACHED
    };

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

    std::vector<std::string> supportedConnectionMode();
}

namespace common
{
    std::string convertToString(fastoredis::connectionTypes t);
    std::string convertToString(fastoredis::ConnectionMode t);
}
