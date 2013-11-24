#pragma once
#include "common/multi_threading/common_headers.hpp"
#include <vector>
namespace common{
namespace multi_threading
{
    template<typename task_t>
    class std_thread_pool_impl
    {
    public:
        typedef task_t task_type;
        std_thread_pool_impl(): stop_(false)
        {

        }
        ~std_thread_pool_impl()
        {
            stop();
        }
        void post(task_type task)
        {
            {
                guard_wraper<mutex_t> lock(queue_mutex);
                tasks.push_back(task);
            }
            condition.notify_one();
        }
        void start(size_t count_threads)
        {
            init_work(count_threads);
        }
        void stop()
        {
            stop_ = true;
            condition.notify_all();
            wait_finish_work();
        }
        void restart()
        {
            stop();
            init_work(workers.size());
        }
    private:
        void init_work(size_t threads)
        {
            workers.clear();
            tasks.clear();
            for(size_t i = 0;i<threads;++i)
            {
                workers.push_back(thread_t(std::bind(&std_thread_pool_impl::run_work,this)));
            }
        }
        void wait_finish_work()
        {
            for(size_t i = 0;i<workers.size();++i)
            {
                workers[i].join();
            }
        }
        void run_work()
        {
              task_type task;
              while(true)
              {
                  {
                      guard_wraper<mutex_t> lock(queue_mutex);
                      while(!stop_ && tasks.empty())
                      {
                          condition.wait(lock);
                      }
                      if(stop_)
                          return;
                      task= tasks.back();
                      tasks.pop_back();
                  }
                  task();
              }
        }
        std::vector<thread_t> workers;
        std::vector<task_type> tasks;
        mutex_t queue_mutex;
        condition_variable_t condition;
        volatile bool stop_;
    };
}
}
