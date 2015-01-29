#pragma once

#include "common/log_levels.h"
#include "common/sprintf.h"
#include "common/patterns/singleton_pattern.h"

namespace common
{
    namespace logging
    {
        class Logger
                : public patterns::LazySingleton<Logger>
        {
            friend class patterns::LazySingleton<Logger>;
        public:
            void printTradeSafe(LEVEL_LOG level, const std::string& data);

        private:
            Logger();
            ~Logger();

            std::ostream* outStream_;
        };        
    }

    template<uint16_t buff_size, typename... Args>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, Args... args)
    {
        char buff[buff_size] = {0};
        SNPrintf(buff, buff_size, fmt, args...);
        logging::Logger::instance().printTradeSafe(level, buff);
    }

    void DEBUG_MSG_PERROR(const char* function, int err);
}
