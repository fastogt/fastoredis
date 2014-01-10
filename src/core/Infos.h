#pragma once

#include <string>

namespace fastoredis
{
    struct DataBaseInfo
    {
        DataBaseInfo(const std::string &name, size_t size);
        std::string name_;
        size_t size_;
    };
}
