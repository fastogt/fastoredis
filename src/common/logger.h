#pragma once

#include "common/log_levels.h"
#include "common/sprintf.h"

namespace common
{
    namespace logging
    {
        void printTradeSafe(LEVEL_LOG level, const char* data);
    }

    template<uint16_t buff_size, typename... Args>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, Args... args)
    {
        char buff[buff_size] = {0};
        SNPrintf(buff, buff_size, fmt, args...);
        logging::printTradeSafe(level, buff);
    }

    void DEBUG_MSG_PERROR(const char* function, int err);
}
