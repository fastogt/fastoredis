#pragma once

/**/

#include "common/types.h"

namespace common
{
    namespace logging
    {
        enum LEVEL_LOG
        {
            NONE,
            DEBUG,
            INFO,
            WARNING,
            L_ERROR,
            CRITICAL
        };

        template<LEVEL_LOG L>
        struct traits_level;

        template<>
        struct traits_level<CRITICAL>
        {
            static const unicode_char *text()
            {
                return UTEXT("Critical");
            }
        };

        template<>
        struct traits_level<L_ERROR>
        {
            static const unicode_char *text()
            {
                return UTEXT("Error");
            }
        };

        template<>
        struct traits_level<WARNING>
        {
            static const unicode_char *text()
            {
                return UTEXT("Warning");
            }
        };

        template<>
        struct traits_level<INFO>
        {
            static const unicode_char *text()
            {
                return UTEXT("Info");
            }
        };

        template<>
        struct traits_level<DEBUG>
        {
            static const unicode_char *text()
            {
                return UTEXT("Debug");
            }
        };

        inline const unicode_char *log_level_to_text(LEVEL_LOG lev)
        {
            switch(lev)
            {
            case CRITICAL:
                return traits_level<CRITICAL>::text();
            case L_ERROR:
                return traits_level<L_ERROR>::text();
            case WARNING:
                return traits_level<WARNING>::text();
            case INFO:
                return traits_level<INFO>::text();
            case DEBUG:
                return traits_level<DEBUG>::text();
            default:
                NOTREACHED();
            }
        }
    }
}
