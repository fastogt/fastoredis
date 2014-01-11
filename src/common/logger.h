#pragma once

#include "common/log_levels.hpp"
#include "common/multi_threading/lockers.h"
#include "common/patterns/singleton_pattern.hpp"

namespace common
{
#ifdef OS_WIN
    typedef multi_threading::lock_free::windows::critical_section::wrap_critical_section locker_t;
#elif defined OS_POSIX
    typedef multi_threading::lock_free::atomic::api_spin_lock locker_t;
#endif
    namespace logging
    {
        class logger:
                public patterns::lazy_singleton<logger>
        {
            friend class patterns::lazy_singleton<logger>;
        public:
            static unicode_ostream& get_cout();
            static unicode_istream& get_cin();
            static unicode_ostream& get_cerr();
            static unicode_char get_endl();

            template<LEVEL_LOG L,typename T>
            void print_trade_safe(const T &t)
            {
                //std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
                //std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
                guard_wraper<locker_t> lock(lock_);
                *m_outStream << traits_level<L>::text << UTEXT(" ") /*<< std::put_time(std::localtime(&now_c), "%F %T :")*/ << t << UTEXT('\n') ;
            }
            template<typename T>
            void print(const T &t,LEVEL_LOG l)
            {
                *m_outStream << log_level_to_text(l) << UTEXT(" ") << t << UTEXT('\n') ;
            }
        #ifdef __GXX_EXPERIMENTAL_CXX0X__
            template<LEVEL_LOG L,unsigned size_of_buffer,typename... TArgs>
            void print_format(const unicode_char *format,const TArgs&... args)
            {
                //std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
                //std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
                unicode_char message[size_of_buffer]={0};
                unicode_sprintf(message,format,args...);
                guard_wraper<locker_t> lock(lock_);
                *m_outStream << traits_level<L>::text  << UTEXT(" ") /*<< std::put_time(std::localtime(&now_c), "%F %T :")*/  << message;
            }
        #endif
        protected:
            ~logger();
        private:
            logger();
            locker_t lock_;
            unicode_ostream * m_outStream;
        };

        template<LEVEL_LOG L,typename T>
        inline void DEBUG_MSG(const T &t)
        {
            return logger::instance().print_trade_safe<L>(t);
        }
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
        template<LEVEL_LOG L,unsigned size_of_buffer,typename... TArgs>
        inline void DEBUG_MSG_FORMAT(const unicode_char *format,const TArgs&... args)
        {
            return logger::instance().print_format<L,size_of_buffer>(format,args...);
        }
    #endif
    }
}
