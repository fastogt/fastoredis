#pragma once

/**/

#include "common/log_levels.h"

#include "common/safe_sprintf.h"

#include "common/multi_threading/lockers.h"

#include "common/patterns/singleton_pattern.h"

namespace common
{
    namespace logging
    {
        class Logger:
                public patterns::lazy_singleton<Logger>
        {
            friend class patterns::lazy_singleton<Logger>;
        public:
            void printTradeSafe(LEVEL_LOG level, const std::string& data);

            template<uint16_t buff_size, typename T>
            void printTradeSafe(LEVEL_LOG level, const char* fmt, T t1)
            {                
                char buff[buff_size] = {0};
                strings::SafeSNPrintf(buff, buff_size, fmt, t1);
                printTradeSafe(level, buff);
            }            

            template<uint16_t buff_size, typename T1, typename T2>
            void printTradeSafe(LEVEL_LOG level, const char* fmt, T1 t1, T2 t2)
            {
                char buff[buff_size] = {0};
                strings::SafeSNPrintf(buff, buff_size, fmt, t1, t2);
                printTradeSafe(level, buff);
            }

            template<uint16_t buff_size, typename T1, typename T2, typename T3>
            void printTradeSafe(LEVEL_LOG level, const char* fmt, T1 t1, T2 t2, T3 t3)
            {
                char buff[buff_size] = {0};
                strings::SafeSNPrintf(buff, fmt, t1, t2, t3);
                printTradeSafe(level, buff);
            }

            template<uint16_t buff_size, typename T1, typename T2, typename T3, typename T4>
            void printTradeSafe(LEVEL_LOG level, const char* fmt, T1 t1, T2 t2, T3 t3, T4 t4)
            {
                char buff[buff_size] = {0};
                strings::SafeSNPrintf(buff, fmt, t1, t2, t3, t4);
                printTradeSafe(level, buff);
            }

        private:
            Logger();
            ~Logger();

#ifdef OS_WIN
    typedef multi_threading::critical_section locker_type;
#elif defined OS_POSIX
    typedef multi_threading::pthread_mutex locker_type;
#endif
            locker_type lock_;
            std::ostream* outStream_;
        };        
    }

    template<uint16_t buff_size, typename T>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, T t)
    {
        return logging::Logger::instance().printTradeSafe<buff_size>(level, fmt, t);
    }

    template<uint16_t buff_size, typename T1, typename T2>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, T1 t1, T2 t2)
    {
        return logging::Logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2);
    }

    template<uint16_t buff_size, typename T1, typename T2, typename T3>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, T1 t1, T2 t2, T3 t3)
    {
        return logging::Logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2, t3);
    }

    template<uint16_t buff_size, typename T1, typename T2, typename T3, typename T4>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const char* fmt, T1 t1, T2 t2, T3 t3, T4 t4)
    {
        return logging::Logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2, t3, t4);
    }

    void DEBUG_MSG_PERROR(const char* function);
}
