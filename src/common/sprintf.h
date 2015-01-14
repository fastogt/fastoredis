#pragma once

#include "common/types.h"

namespace common
{
    template<typename T>
    inline T normalize(T t)
    {
        return t;
    }

    inline const char* normalize(const std::string& text)
    {
        return text.c_str();
    }

    inline const char* normalize(const buffer_type& buffer)
    {
        return (const char*)buffer.c_str();
    }

    template<typename... Args>
    inline int SNPrintf(char* buff, uint16_t buff_size, const char* fmt, Args... args)
    {
        int res = snprintf(buff, buff_size, fmt, normalize(args)...);
        DCHECK(res != -1 && res < buff_size);
        return res;
    }
}  // namespace common
