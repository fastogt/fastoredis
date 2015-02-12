#pragma once

#include "common/multi_threading/types.h"

#include <vector>

namespace common
{
    namespace multi_threading
    {
        template<typename task_t>
        class std_thread_pool_impl
        {
        public:
            typedef task_t task_type;
            std_thread_pool_impl()
                : stop_(false)
            {

            }

            ~std_thread_pool_impl()
            {
                stop();
            }

            void post(task_type task)
            {
                {
                    unique_lock<mutex_t> lock(queue_mutex);
                    tasks.push_back(task);
                }
                condition.notify_one();
            }

            void start(uint16_t count_threads)
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

            void init_work(uint16_t threads)
            {
                workers.clear();
                tasks.clear();
                for(uint16_t i = 0; i < threads; ++i){
#ifdef BOOST_ENABLED
                    workers.push_back(thread_t(boost::bind(&std_thread_pool_impl::run_work, this)));
#elif defined(HAVE_CXX_STANDART)
                    workers.push_back(thread_t(std::bind(&std_thread_pool_impl::run_work, this)));
#else
    #error implement bind functionality
#endif
                }
            }

            void wait_finish_work()
            {
                for(uint16_t i = 0;i<workers.size();++i){
                    workers[i].join();
                }
            }

            void run_work()
            {
                  task_type task;
                  while(true){
                      {
                          unique_lock<mutex_t> lock(queue_mutex);
                          while(!stop_ && tasks.empty()){
                              condition.wait(lock);
                          }
                          if(stop_){
                              return;
                          }
                          task = tasks.back();
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
