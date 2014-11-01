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
}
