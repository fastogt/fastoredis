#include "core/connection_types.h"

#include "common/utils.h"

namespace
{
    const char *connnectionType[fastoredis::REDIS+1] = { "Unknown", "Redis" };
}

namespace fastoredis
{
    std::vector<std::string> supportedConnectionTypes()
    {
        return common::utils::enums::convertToVector(connnectionType);
    }

    std::string toStdString(connectionTypes t)
    {
        std::string result;
        int count = sizeof(connnectionType)/sizeof(*connnectionType);
        if(t < count){
            result = connnectionType[t];
        }
        return result;
    }

    connectionTypes toConnectionType(const std::string &text)
    {
        return common::utils::enums::findTypeInArray<connectionTypes>(connnectionType,text.c_str());
    }
}
