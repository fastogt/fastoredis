#pragma once
#include "common_headers.hpp"
#include <vector>
#include <boost/asio.hpp>
namespace common{
namespace multi_threading
{
    template<typename task_t>
    class boost_thread_pool_impl
    {
        typedef std::shared_ptr<thread_t> value_type;
    public:
        typedef task_t task_type;
        boost_thread_pool_impl():work_(io_service)
        {

        }
        void post(task_type task)
        {
             io_service.post(task);
        }
        void start(size_t count_threads)
        {
            for(size_t i=0 ; i<count_threads ; ++i)
            {
                typedef size_t(boost::asio::io_service::*run_type)();
                value_type new_thread(new thread_t(std::bind(static_cast<run_type>(&boost::asio::io_service::run), std::ref(io_service))));
                grp.push_back(new_thread);
            }
        }
        ~boost_thread_pool_impl()
        {
            io_service.stop();
            for(int i = 0;i<grp.size();i++)
            {
                grp[i]->join();
            }
        }
        std::vector<value_type> grp;
        boost::asio::io_service io_service;
        const boost::asio::io_service::work work_;
    };
}
}
