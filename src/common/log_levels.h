#pragma once

/**/

#include "common/types.h"

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

        inline string16 log_level_to_text(LEVEL_LOG lev)
        {
            switch(lev)
            {
            case L_CRITICAL:
                return UTEXT("Critical");
            case L_ERROR:
                return UTEXT("Error");
            case L_WARNING:
                return UTEXT("Warning");
            case L_INFO:
                return UTEXT("Info");
            case L_DEBUG:
                return UTEXT("Debug");
            default:
                NOTREACHED();
            }
        }
    }
}
