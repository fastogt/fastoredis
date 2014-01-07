#pragma once

#include "global/global.h"

namespace fastoredis
{
    struct DataBaseInfo
    {
        DataBaseInfo(const std::string &name, size_t size);
        std::string name_;
        size_t size_;
    };
}
