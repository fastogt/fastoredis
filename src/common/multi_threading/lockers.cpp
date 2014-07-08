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
namespace lock_free //unix
{
    namespace unix_api
    {
        namespace atomic
        {
            unix_spin_lock::unix_spin_lock()
            {
                pthread_spin_init(&spinlock,0);
            }
            void unix_spin_lock::lock()
            {
                pthread_spin_lock(&spinlock);
            }
            void unix_spin_lock::unlock()
            {
                pthread_spin_unlock(&spinlock);
            }
            unix_spin_lock::~unix_spin_lock()
            {
                pthread_spin_destroy(&spinlock);
            }			
        }
        namespace mutex
        {
            wrap_mutex::wrap_mutex()throw()
            {
                pthread_mutex_init(&mutex_, NULL);
            }
            void wrap_mutex::lock() throw()
            {
                 pthread_mutex_lock (&mutex_);
            }
            void wrap_mutex::unlock() throw()
            {
                pthread_mutex_unlock (&mutex_);
            }
            wrap_mutex::~wrap_mutex()
            {
                pthread_mutex_destroy(&mutex_);
            }
        }
    }
}
}
#elif defined _WIN32
#include <windows.h>
namespace multi_threading
{
namespace lock_free//windows
{
    namespace windows
    {
        namespace critical_section
        {
                wrap_critical_section::wrap_critical_section()
                {
                    memset(&m_sec, 0, sizeof(CRITICAL_SECTION));
                    InitializeCriticalSection(&m_sec);
                }
                wrap_critical_section::~wrap_critical_section()
                {
                    DeleteCriticalSection(&m_sec);
                }
                void wrap_critical_section::lock()
                {
                    EnterCriticalSection(&m_sec);
                }
                void wrap_critical_section::unlock()
                {
                    LeaveCriticalSection(&m_sec);
                }
        }
        namespace atomic
        {
                void win_spin_lock::lock()
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

                void win_spin_lock::unlock()
                {
                    lock_.dest_=0;
                    //InterlockedCompareExchange(&lock_.dest_, 0, GetCurrentThreadId());
                }
        }
    }
}
}
#endif
}
