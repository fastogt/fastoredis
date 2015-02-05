#pragma once

#include <string>

#include "common/macros.h"

namespace common
{
    namespace logging
    {
        enum LEVEL_LOG
        {
            L_NONE,
            L_DEBUG,
            L_INFO,
            L_WARNING,
            L_ERROR,
            L_CRITICAL
        };

        static const char* level_names[] = { "NONE", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };

        inline const char* log_level_to_text(LEVEL_LOG lev)
        {
            return level_names[lev];
        }
    }
}
