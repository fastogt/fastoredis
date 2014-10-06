#include "core/connection_types.h"

#include "common/utils.h"

namespace
{
    const common::char16 *connnectionType[fastoredis::REDIS+1] = { UTEXT("Unknown"), UTEXT("Redis") };
}

namespace fastoredis
{
    std::vector<common::string16> supportedConnectionTypes()
    {
        return common::utils::enums::convertToVector(connnectionType);
    }
}

namespace common
{

    template<>
    fastoredis::connectionTypes convertFromString16(const string16& text)
    {
        return common::utils::enums::findTypeInArray<fastoredis::connectionTypes>(connnectionType, text.c_str());
    }

    string16 convertToString16(fastoredis::connectionTypes t)
    {
        int count = sizeof(connnectionType)/sizeof(*connnectionType);
        if(t < count){
            return connnectionType[t];
        }
        return string16();
    }
}
