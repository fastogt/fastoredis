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

    static const std::vector<std::string> connnectionType = { "Unknown", "Redis", "Memcached" };

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

    static const std::vector<std::string> connnectionMode = { "Latency mode", "Slave mode", "Get RDB mode", "Pipe mode",  "Find big keys mode", "Stat mode", "Scan mode", "Intaractive mode" };
}

namespace common
{
    std::string convertToString(fastoredis::connectionTypes t);
    std::string convertToString(fastoredis::ConnectionMode t);
}
