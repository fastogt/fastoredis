#pragma once
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
typedef std::thread thread_t;
typedef std::mutex mutex_t;
template<typename type>
using guard_wraper = std::unique_lock<type>;
typedef std::condition_variable condition_variable_t;
#elif defined BOOST_SUPPORT_ENABLED
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/condition.hpp>
typedef boost::thread thread_t;
typedef boost::mutex mutex_t;
template<typename T>
struct guard_wraper
        : public boost::unique_lock<T>
{
};
typedef boost::condition_variable condition_variable_t;
#endif

namespace common
{
    namespace multi_threading
    {

    }
}
