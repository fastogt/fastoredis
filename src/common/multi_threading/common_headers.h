#pragma once

/**/
#ifdef BOOST_ENABLED

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/condition.hpp>

namespace common
{
    namespace multi_threading
    {
        typedef boost::thread thread_type;
        typedef boost::mutex mutex_type;

        template<typename T>
        struct guard_wraper
                : public boost::unique_lock<T>
        {
            guard_wraper(T &t)
                : boost::unique_lock<T>(t)
            {

            }
        };

        typedef boost::condition_variable condition_variable_type;
    }
}

#else

#endif
