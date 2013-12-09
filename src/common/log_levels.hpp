#pragma once

#include "common/macros.h"

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
            ERROR,
            CRITICAL
        };

        template<LEVEL_LOG L>
        struct traits_level;
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
        template<>
        struct traits_level<CRITICAL>
        {
            static constexpr const unicode_char *text =  UTEXT("Critical");
        };
        template<>
        struct traits_level<ERROR_>
        {
            static constexpr const unicode_char *text = UTEXT("Error");
        };
        template<>
        struct traits_level<WARNING>
        {
            static constexpr const unicode_char *text = UTEXT("Warning");
        };
        template<>
        struct traits_level<INFO>
        {
            static constexpr const unicode_char *text = UTEXT("Info");
        };
        template<>
        struct traits_level<DEBUG_>
        {
            static constexpr const unicode_char *text = UTEXT("Debug");
        };
    #else
        template<>
        struct traits_level<CRITICAL>
        {
            static const unicode_char *text()
            {
                return UTEXT("Critical");
            }
        };
        template<>
        struct traits_level<ERROR>
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
    #endif
        inline const unicode_char *log_level_to_text(LEVEL_LOG lev)
        {
            switch(lev)
            {
            case CRITICAL:
                return traits_level<CRITICAL>::text();
            case ERROR:
                return traits_level<ERROR>::text();
            case WARNING:
                return traits_level<WARNING>::text();
            case INFO:
                return traits_level<INFO>::text();
            case DEBUG:
                return traits_level<DEBUG>::text();
            default:
                DCHECK(false);
            }
        }
    }
}
