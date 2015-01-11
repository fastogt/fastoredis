#pragma once

#include <stdio.h>

#include "common/log_levels.h"
#include "common/types.h"
#include "common/patterns/singleton_pattern.h"

namespace common
{
    namespace logging
    {
        class Logger
                : public patterns::lazy_singleton<Logger>
        {
            friend class patterns::lazy_singleton<Logger>;
        public:
            void printTradeSafe(LEVEL_LOG level, const std::string& data);

        private:
            Logger();
            ~Logger();

            std::ostream* outStream_;
        };        
    }

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

    template<uint16_t buff_size, typename... Args>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, Args... args)
    {
        char buff[buff_size] = {0};
        int res = snprintf(buff, buff_size, fmt, normalize(args)...);
        DCHECK(res != -1 && res < buff_size);
        logging::Logger::instance().printTradeSafe(level, buff);
    }

    void DEBUG_MSG_PERROR(const char* function, int err);
}
