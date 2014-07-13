#pragma once

#include "common/multi_threading/common_headers.h"

#ifdef OS_WIN
#include <windows.h>
#endif

namespace common
{
#ifdef OS_WIN
    namespace multi_threading
    {
        namespace lock_free//windows
        {
            namespace windows
            {
                namespace critical_section
                {
                        class wrap_critical_section//398
                        {
                        public:
                            wrap_critical_section();
                            ~wrap_critical_section();
                            void lock();
                            void unlock();
                        private:
                            CRITICAL_SECTION m_sec;
                        };
                }
                namespace atomic
                {
                        class win_spin_lock//410
                        {
                        public:
                            win_spin_lock():m_iterations(0){}
                            void lock();
                            void unlock();
                        private:
                            struct spin_lock
                            {
                                volatile long dest_;
                                long exchange_;
                                spin_lock():dest_(0),exchange_(SeedVal){}
                            } lock_;
                            unsigned int m_iterations;
                            static const int SeedVal = 100;
                        };
                }
            }
            namespace atomic
            {
                    typedef windows::atomic::win_spin_lock api_spin_lock;
            }
        }
    }
#elif defined OS_POSIX
    namespace multi_threading
    {
        namespace lock_free //unix
        {
            namespace unix_api
            {
                namespace atomic
                {
                    class unix_spin_lock
                    {
                    public:
                        unix_spin_lock();
                        void lock();
                        void unlock();
                        ~unix_spin_lock();
                    private:
                        pthread_spinlock_t spinlock;
                    };
                }
                namespace mutex
                {
                    struct wrap_mutex
                    {
                        wrap_mutex() throw();
                        ~wrap_mutex();
                        void lock() throw();
                        void unlock() throw();
                        pthread_mutex_t mutex_;
                    };
                }
            }

            namespace atomic
            {
                typedef unix_api::atomic::unix_spin_lock api_spin_lock;
            }

            namespace mutex
            {
                typedef unix_api::mutex::wrap_mutex api_mutex;
            }
        }
    }
#endif
}
