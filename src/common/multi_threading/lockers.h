#pragma once

#include "common/multi_threading/common_headers.h"

#ifdef OS_WIN
#include <windows.h>
#elif defined(OS_MAC)
#include <libkern/OSAtomic.h>
#endif

namespace common
{
#ifdef OS_WIN
    namespace multi_threading
    {
        class critical_section//398
        {
        public:
            critical_section();
            ~critical_section();
            void lock();
            void unlock();
        private:
            CRITICAL_SECTION m_sec;
        };

        class spin_lock//410
        {
        public:
            spin_lock():m_iterations(0){}
            void lock();
            void unlock();
        private:
            struct spin_lock_p
            {
                volatile long dest_;
                long exchange_;
                spin_lock_p():dest_(0),exchange_(SeedVal){}
            } lock_;
            unsigned int m_iterations;
            static const int SeedVal = 100;
        };
    }

#elif defined OS_POSIX
    namespace multi_threading
    {
                    class spin_lock
                    {
                    public:
                        spin_lock();
                        void lock();
                        void unlock();
                        ~spin_lock();
                    private:
#ifdef OS_MAC
                        OSSpinLock spinlock;
#else
                        pthread_spinlock_t spinlock;
#endif
                    };

                    struct pthread_mutex
                    {
                        pthread_mutex();
                        ~pthread_mutex();
                        void lock();
                        void unlock();
                        pthread_mutex_t mutex_;
                    };
    }
#endif
}
