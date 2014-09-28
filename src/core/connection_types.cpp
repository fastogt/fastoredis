#include "core/connection_types.h"

#include "common/utils.h"

namespace
{
    const unicode_char *connnectionType[fastoredis::REDIS+1] = { UTEXT("Unknown"), UTEXT("Redis") };
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
    fastoredis::connectionTypes convertfromString(const unicode_string& text)
    {
        return common::utils::enums::findTypeInArray<fastoredis::connectionTypes>(connnectionType, text.c_str());
    }

    unicode_string convert2string(fastoredis::connectionTypes t)
    {
        int count = sizeof(connnectionType)/sizeof(*connnectionType);
        if(t < count){
            return connnectionType[t];
        }
        return unicode_string();
    }
}
