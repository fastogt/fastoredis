#pragma once

/**/

#include "common/log_levels.h"

#include "common/multi_threading/lockers.h"

#include "common/patterns/singleton_pattern.h"

namespace common
{
    namespace logging
    {
        class logger:
                public patterns::lazy_singleton<logger>
        {
            friend class patterns::lazy_singleton<logger>;
        public:
            static unicode_ostream& cout();
            static unicode_istream& cin();
            static unicode_ostream& cerr();
            static unicode_char endl();

            void printTradeSafe(LEVEL_LOG level, const unicode_string& data);

            template<uint16_type buff_size, typename T>
            void printTradeSafe(LEVEL_LOG level, const unicode_char* fmt, T t1)
            {                
                unicode_char buff[buff_size] = {0};
                unicode_sprintf(buff, fmt, t1);
                printTradeSafe(level, buff);
            }            

            template<uint16_type buff_size, typename T1, typename T2>
            void printTradeSafe(LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2)
            {
                unicode_char buff[buff_size] = {0};
                unicode_sprintf(buff, fmt, t1, t2);
                printTradeSafe(level, buff);
            }

            template<uint16_type buff_size, typename T1, typename T2, typename T3>
            void printTradeSafe(LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2, T3 t3)
            {
                unicode_char buff[buff_size] = {0};
                unicode_sprintf(buff, fmt, t1, t2, t3);
                printTradeSafe(level, buff);
            }

            template<uint16_type buff_size, typename T1, typename T2, typename T3, typename T4>
            void printTradeSafe(LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2, T3 t3, T4 t4)
            {
                unicode_char buff[buff_size] = {0};
                unicode_sprintf(buff, fmt, t1, t2, t3, t4);
                printTradeSafe(level, buff);
            }

        private:
            logger();
            ~logger();

#ifdef OS_WIN
    typedef multi_threading::lock_free::windows::critical_section::wrap_critical_section locker_type;
#elif defined OS_POSIX
    typedef multi_threading::lock_free::atomic::api_spin_lock locker_type;
#endif
            locker_type lock_;
            unicode_ostream * outStream_;
        };        
    }

    template<uint16_type buff_size, typename T>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const unicode_char* fmt, T t)
    {
        return logging::logger::instance().printTradeSafe<buff_size>(level, fmt, t);
    }

    template<uint16_type buff_size, typename T1, typename T2>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2)
    {
        return logging::logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2);
    }

    template<uint16_type buff_size, typename T1, typename T2, typename T3>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2, T3 t3)
    {
        return logging::logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2, t3);
    }

    template<uint16_type buff_size, typename T1, typename T2, typename T3, typename T4>
    inline void DEBUG_MSG_FORMAT(logging::LEVEL_LOG level, const unicode_char* fmt, T1 t1, T2 t2, T3 t3, T4 t4)
    {
        return logging::logger::instance().printTradeSafe<buff_size>(level, fmt, t1, t2, t3, t4);
    }

    void DEBUG_MSG_PERROR(const unicode_char* function);
}
