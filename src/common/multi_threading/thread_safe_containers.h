#pragma once

#include <vector>
#include <list>
#include <algorithm>

#include "common/multi_threading/lockers.h"

namespace common
{
    namespace multi_threading
    {
        namespace thread_safe_containers
        {
            namespace  standart_library
            {
                template <typename type_t>
                class vector
                {
                public:
                    typedef std::vector<type_t> listItem;
                    explicit vector():m_stopConditionWait(false)
                    {
                    }

                    void send(type_t val)
                    {
                        guard_wraper<mutex_type> lock(m_mutex);
                        m_listItem.push_back(val);
                        m_conditionVariable.notify_one();
                    }

                    bool receive(type_t &val)
                    {
                        guard_wraper<mutex_type> lock(m_mutex);
                        while(m_listItem.empty()){
                            if(m_stopConditionWait){
                                return false;
                            }
                            else{
                                m_conditionVariable.wait(lock);
                            }
                        }
                        val = *m_listItem.begin();
                        m_listItem.erase(m_listItem.begin());
                        return true;
                    }

                    int size()const
                    {
                        return m_listItem.size();
                    }

                    void interrupt()
                    {
                        guard_wraper<mutex_t> lock(m_mutex);
                        m_stopConditionWait=true;
                        m_conditionVariable.notify_all();
                    }

                    bool empty()const
                    {
                        return m_listItem.empty();
                    }

                    ~vector()
                    {
                        interrupt();
                    }

                private:
                    volatile bool m_stopConditionWait;
                    listItem m_listItem;
                    mutex_type m_mutex;
                    condition_variable_type m_conditionVariable;
                };
            }

#ifdef OS_WIN
            #include <Windows.h>
            namespace windows
            {
                template<typename type_t>
                struct vector
                {
                typedef type_t value_type;
                typedef std::vector<value_type> listItem;
                vector()
                {
                    hReadyForProcessing = CreateEvent(
                    NULL, // default security attributes
                    FALSE, // manual-reset event
                    FALSE, // initial state is signaled
                    0); //No name
                    hTerminate = CreateEvent(
                    NULL, // default security attributes
                    FALSE, // manual-reset event
                    FALSE, // initial state is signaled
                    0); //No name
                }
                void send(value_type val)
                {
                    m_listItem.push_back(val);
                    if(m_listItem.size()==1)
                    {
                        SetEvent(hReadyForProcessing);
                    }
                }
                bool receive(value_type &result)
                {
                    while(m_listItem.empty())
                    {
                        if(hTerminate&&hReadyForProcessing)
                        {
                            HANDLE hEvents[2] = {hReadyForProcessing, hTerminate};
                            DWORD dwRes = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
                            if (dwRes!=WAIT_OBJECT_0)
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                    result=*m_listItem.begin();
                    m_listItem.erase(m_listItem.begin());
                    return true;
                }
                void interrupt()
                {
                    SetEvent(hTerminate);
                    CloseHandle(hTerminate);
                    hTerminate = nullptr;
                    CloseHandle(hReadyForProcessing);
                    hReadyForProcessing = nullptr;
                }
                int size()const
                {
                    return m_listItem.size();
                }
                bool empty()const
                {
                    return m_listItem.empty();
                }
                ~vector()
                {
                    interrupt();
                }
                private:
                    listItem m_listItem;
                    HANDLE hReadyForProcessing;
                    HANDLE hTerminate;
                };
                template <typename type_t>
                class thread_safe_list : protected std::list<type_t>
                {
                public:
                typedef guard_wraper<lock_free::windows::critical_section::wrap_critical_section> locker_t;
                typedef std::list<type_t> base_class;
                thread_safe_list(int nMaxCount)
                {
                    m_bOverflow = false;
                    m_hSemaphore = ::CreateSemaphore(NULL,0,nMaxCount,NULL);
                }
                ~thread_safe_list()
                {
                    ::CloseHandle(m_hSemaphore);
                    m_hSemaphore = NULL;
                }
                void push(const type_t& c)
                {
                    {
                        locker_t lock( m_Crit );
                        base_class::push_back( c );
                    }
                    if (!::ReleaseSemaphore(m_hSemaphore, 1, NULL))
                    {
                        // If the semaphore is full, then take back the entry.
                        base_class::pop_back();
                        if (GetLastError() == ERROR_TOO_MANY_POSTS)
                        {
                            m_bOverflow = true;
                        }
                    }
                }
                bool pop(type_t& c)
                {
                    locker_t lock( m_Crit );
                    if (base_class::empty())
                    {
                        while (::WaitForSingleObject(m_hSemaphore, 0) != WAIT_TIMEOUT)
                        1;
                        return false;
                    }
                        c = base_class::front();
                        base_class::pop_front();
                    return true;
                }
                void clear()
                {
                    locker_t lock( m_Crit );
                    for (DWORD i=0; i<base_class::size(); i++)
                    WaitForSingleObject(m_hSemaphore, 0);
                    base_class::clear();
                    m_bOverflow = false;
                }
                bool overflow()
                {
                    return m_bOverflow;
                }
                HANDLE GetWaitHandle() { return m_hSemaphore; }
                private:
                    HANDLE m_hSemaphore;
                    lock_free::windows::critical_section::wrap_critical_section m_Crit;
                    bool m_bOverflow;
                };
                }
                template<typename type_t>
                struct thread_safe_containers
                {
                    typedef windows::vector<type_t> api_vector;
                    typedef windows::thread_safe_list<type_t> api_thread_safe_list;
                };
#elif defined OS_POSIX
            namespace unix
            {
                template<typename type_t,typename list_items_t = std::vector<type_t> >
                struct container
                {
                    typedef type_t value_type;
                    typedef guard_wraper<lock_free::unix::mutex::wrap_mutex> locker_t;

                    container():stop_(false)
                    {
                    }

                    void send(value_type val)
                    {
                        locker_t lock(mutex_);
                        items_array_.push_back(val);
                        pthread_cond_signal(&cond_variable_);
                    }

                    bool receive(value_type &val)
                    {
                        locker_t lock(mutex_);
                        while(items_array_.empty())
                        {
                            if(stop_)
                            {
                                return false;
                            }
                            else
                            {
                                pthread_cond_wait(&cond_variable_, &mutex_.mutex_);
                            }
                        }
                        val=*items_array_.begin();
                        items_array_.erase(items_array_.begin());
                        return true;
                    }

                    void interrupt()
                    {
                        locker_t lock(mutex_);
                        stop_=true;
                        pthread_cond_signal(&cond_variable_);
                    }

                    int size()const
                    {
                        return items_array_.size();
                    }

                    bool empty()const
                    {
                        return items_array_.empty();
                    }

                    ~container()
                    {
                        interrupt();
                    }

                private:
                    list_items_t items_array_;
                    lock_free::unix::mutex::wrap_mutex mutex_;
                    volatile bool stop_;
                    pthread_cond_t  cond_variable_;
                };
            }

            template<typename type_t,typename list_items_t = std::vector<type_t> >
            using container = unix::container<type_t,list_items_t>;
#endif
        }

    }
}
