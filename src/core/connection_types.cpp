#include "core/connection_types.h"

#include "common/utils.h"

namespace
{
    const char* connnectionType[fastoredis::REDIS + 1] = { "Unknown", "Redis" };
    const char* connnectionMode[fastoredis::IntaractiveMode + 1] =
    {
        "Latency mode", "Slave mode", "Get RDB mode", "Pipe mode",  "Find big keys mode", "Stat mode", "Scan mode", "Intaractive mode"
    };
}

namespace fastoredis
{
    std::vector<std::string> supportedConnectionTypes()
    {
        return common::utils::enums::convertToVector(connnectionType);
    }

    std::vector<std::string> supportedConnectionMode()
    {
        return common::utils::enums::convertToVector(connnectionMode);
    }
}

namespace common
{

    template<>
    fastoredis::connectionTypes convertFromString(const std::string& text)
    {
        return common::utils::enums::findTypeInArray<fastoredis::connectionTypes>(connnectionType, text.c_str());
    }

    std::string convertToString(fastoredis::connectionTypes t)
    {
        int count = sizeof(connnectionType)/sizeof(*connnectionType);
        if(t < count){
            return connnectionType[t];
        }
        return std::string();
    }

    std::string convertToString(fastoredis::ConnectionMode t)
    {
        int count = sizeof(connnectionMode)/sizeof(*connnectionMode);
        if(t < count){
            return connnectionMode[t];
        }
        return std::string();
    }
}
