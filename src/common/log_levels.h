#pragma once

/**/

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

        inline std::string log_level_to_text(LEVEL_LOG lev)
        {
            switch(lev)
            {
            case L_CRITICAL:
                return "Critical";
            case L_ERROR:
                return "Error";
            case L_WARNING:
                return "Warning";
            case L_INFO:
                return "Info";
            case L_DEBUG:
                return "Debug";
            default:
                NOTREACHED();
                return std::string();
            }
        }
    }
}
