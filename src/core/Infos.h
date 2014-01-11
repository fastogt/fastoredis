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

    inline bool operator ==(const DataBaseInfo &lhs, const DataBaseInfo &rhs)
    {
        return lhs.name_ == rhs.name_ && lhs.size_ == rhs.size_;
    }
}
