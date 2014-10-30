#pragma once

/**/

#include <vector>
#include <pthread.h>
#include <signal.h>

namespace common
{
    namespace multi_threading
    {
        template<typename task_t, size_t stack_size, size_t... sign>
        class pthread_thread_pool_impl
        {
        public:
            typedef task_t task_type;
            static_assert(PTHREAD_STACK_MIN <= stack_size,"stack_size can't be less than PTHREAD_STACK_MIN ");

            pthread_thread_pool_impl()
                : stop_(false)
            {
                sigset_t signal_mask;
                utils::signal::init_sigset(&signal_mask, sign...);
                pthread_sigmask(SIG_SETMASK, &signal_mask, NULL);
                pthread_attr_init(&attr_);
                pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_JOINABLE);
                pthread_attr_setstacksize(&attr_, stack_size);
                pthread_attr_setguardsize(&attr_,0);
                pthread_mutex_init(&queue_mutex_, NULL);
                pthread_cond_init(&condition_,NULL);
                pthread_mutex_init(&workers_mutex_, NULL);
            }

            static void *thread_func(void *d)
            {
                (static_cast<pthread_thread_pool_impl *>(d))->run_work();
                return NULL;
            }

            ~pthread_thread_pool_impl()
            {
                stop();
                pthread_mutex_destroy(&queue_mutex_);
                pthread_cond_destroy(&condition_);
                pthread_attr_destroy(&attr_);
                pthread_mutex_destroy(&workers_mutex_);
            }

            void post(task_type task)
            {
                pthread_mutex_lock (&queue_mutex_);
                tasks_.push_back(task);
                pthread_cond_signal(&condition_);
                pthread_mutex_unlock (&queue_mutex_);
            }

            void resize(size_t num,bool is_reload=false)
            {
                if(is_reload||stop_)
                {
                    if(!stop_)
                    {
                        stop();
                    }
                    start(num);
                }
                else
                {
                    pthread_mutex_lock(&workers_mutex_);
                    for(size_t i = workers_.size();i<num;++i)
                    {
                        pthread_t thread;
                        pthread_create(&thread, &attr_,pthread_thread_pool_impl::thread_func, this);
                        workers_.push_back(thread);
                    }
                    pthread_mutex_unlock(&workers_mutex_);
                }
            }

            void start(size_t count_threads)
            {
                init_work(count_threads);
            }

            void stop()
            {
                stop_ = true;
                pthread_cond_broadcast(&condition_);
                wait_finish_work();
            }

            void restart()
            {
                stop();
                init_work(workers_.size());
            }

        private:
            void init_work(size_t threads)
            {
                pthread_mutex_lock(&workers_mutex_);
                workers_.resize(threads);
                workers_.clear();
                tasks_.clear();
                for(size_t i = 0; i<threads; ++i)
                {
                    pthread_t thread;
                    pthread_create(&thread, &attr_, pthread_thread_pool_impl::thread_func, this);
                    workers_.push_back(thread);
                }
                pthread_mutex_unlock(&workers_mutex_);
            }

            void wait_finish_work()
            {
                pthread_mutex_lock(&workers_mutex_);
                for(size_t i = 0;i<workers_.size();++i)
                {
                    pthread_join(workers_[i], NULL);
                }
                workers_.clear();
                pthread_mutex_unlock(&workers_mutex_);
            }

            void run_work()
            {
                  task_type task;
                  while(!stop_)
                  {
                          pthread_mutex_lock (&queue_mutex_);
                          while(!stop_ && tasks_.empty())
                          {
                              pthread_cond_wait(&condition_, &queue_mutex_);
                          }
                          if(stop_)
                          {
                              pthread_mutex_unlock (&queue_mutex_);
                              return;
                          }
                          task = tasks_.back();
                          tasks_.pop_back();
                          pthread_mutex_unlock (&queue_mutex_);
                      task();
                  }
                  pthread_exit(NULL);
            }

            pthread_attr_t attr_;
            std::vector<pthread_t> workers_;
            std::vector<task_type> tasks_;

            pthread_mutex_t queue_mutex_;
            pthread_cond_t condition_;
            pthread_mutex_t workers_mutex_;
            volatile bool stop_;
        };

    }
}
