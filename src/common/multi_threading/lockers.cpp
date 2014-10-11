#include "common/multi_threading/lockers.h"

namespace
{
    const unsigned int YIELD_ITERATION = 30; // yeild after 30 iterations
    const unsigned int MAX_SLEEP_ITERATION = 40;
}

namespace common
{
#ifdef OS_POSIX
namespace multi_threading
{
            spin_lock::spin_lock()
            {
#ifdef OS_LINUX
                pthread_spin_init(&spinlock,0);
#else
#endif
            }
            void spin_lock::lock()
            {
#ifdef OS_LINUX
                pthread_spin_lock(&spinlock);
#else
#endif
            }
            void spin_lock::unlock()
            {
#ifdef OS_LINUX
                pthread_spin_unlock(&spinlock);
#else
#endif
            }
            spin_lock::~spin_lock()
            {
#ifdef OS_LINUX
                pthread_spin_destroy(&spinlock);
#else
#endif
            }			

            pthread_mutex::pthread_mutex()
            {
                pthread_mutex_init(&mutex_, NULL);
            }
            void pthread_mutex::lock()
            {
                 pthread_mutex_lock (&mutex_);
            }
            void pthread_mutex::unlock()
            {
                pthread_mutex_unlock (&mutex_);
            }
            pthread_mutex::~pthread_mutex()
            {
                pthread_mutex_destroy(&mutex_);
            }
}
#elif defined OS_WIN
#include <windows.h>
    namespace multi_threading
    {
        critical_section::critical_section()
        {
            memset(&m_sec, 0, sizeof(CRITICAL_SECTION));
            InitializeCriticalSection(&m_sec);
        }
        critical_section::~critical_section()
        {
            DeleteCriticalSection(&m_sec);
        }
        void critical_section::lock()
        {
            EnterCriticalSection(&m_sec);
        }
        void critical_section::unlock()
        {
            LeaveCriticalSection(&m_sec);
        }

        void spin_lock::lock()
        {
            m_iterations = 0;
            long cur_th = GetCurrentThreadId();
            while(true)
            {
                if(lock_.dest_ == cur_th)
                    break;

                if(InterlockedCompareExchange(&lock_.dest_, cur_th, 0) == 0)
                {
                    break;
                }
                while(lock_.dest_)
                {
                    if(m_iterations >= YIELD_ITERATION)
                    {
                        if(m_iterations + YIELD_ITERATION >= MAX_SLEEP_ITERATION)
                            Sleep(0);

                        if(m_iterations >= YIELD_ITERATION && m_iterations < MAX_SLEEP_ITERATION)
                        {
                            m_iterations = 0;
                            SwitchToThread();
                        }
                    }
                    m_iterations++;
                    //please test behavior
                   /* if(thread_t::hardware_concurrency() > 1) { YieldProcessor(); }
                    else { SwitchToThread(); }*/
                }
            }
        }

        void spin_lock::unlock()
        {
            lock_.dest_=0;
            //InterlockedCompareExchange(&lock_.dest_, 0, GetCurrentThreadId());
        }
    }
#endif
}
