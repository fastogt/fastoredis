#include "core/connection_types.h"

namespace
{
    const std::vector<std::string> connnectionMode = { "Latency mode", "Slave mode", "Get RDB mode", "Pipe mode",  "Find big keys mode", "Stat mode", "Scan mode", "Intaractive mode" };
}

namespace common
{
    template<>
    fastoredis::connectionTypes convertFromString(const std::string& text)
    {
        for (uint32_t i = 0; i < fastoredis::connnectionType.size(); ++i){
            if (text == fastoredis::connnectionType[i]){
                return static_cast<fastoredis::connectionTypes>(i);
            }
        }

        return fastoredis::DBUNKNOWN;
    }

    std::string convertToString(fastoredis::connectionTypes t)
    {
        return fastoredis::connnectionType[t];
    }

    std::string convertToString(fastoredis::ConnectionMode t)
    {
        return connnectionMode[t];
    }
}
