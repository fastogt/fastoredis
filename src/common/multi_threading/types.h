#pragma once

#ifdef BOOST_ENABLED

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

namespace common
{
    namespace multi_threading
    {
        typedef boost::thread thread_t;
        typedef boost::mutex mutex_t;
        typedef boost::condition_variable condition_variable_t;

        template<typename T>
        struct unique_lock
                : public boost::unique_lock<T>
        {
            unique_lock(T &t)
                : boost::unique_lock<T>(t)
            {

            }
        };        
    }
}

#elif defined(HAVE_CXX_STANDART)

#include <thread>
#include <mutex>
#include <condition_variable>

namespace common
{
    namespace multi_threading
    {
        typedef std::thread thread_t;
        typedef std::mutex mutex_t;
        typedef std::condition_variable condition_variable_t;

        template<typename T>
        struct unique_lock
                : public std::unique_lock<T>
        {
            unique_lock(T& t)
                : std::unique_lock<T>(t)
            {

            }
        };
    }
}

#endif
