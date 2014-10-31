#pragma once

#include <pthread.h>
#include <signal.h>
#include <memory>

#include "common/mpl.h"

namespace common
{
    namespace multi_threading
    {
        template<typename type_t, size_t stack_size, size_t... sign>
        class pthread_wrapper
        {
        public:
            static_assert(PTHREAD_STACK_MIN <= stack_size, "stack_size can't be more than PTHREAD_STACK_MIN");
            typedef std::function<void(type_t&)> start_func_type;
            typedef std::function<void(type_t&)> stop_func_type;

            template<typename... args_t>
            pthread_wrapper(start_func_type start_func, stop_func_type stop_func, const args_t&... args)
                :var_(args...),start_func_(start_func),stop_func_(stop_func),thread_handle_(0)
            {
                sigset_t signal_mask;
                sigemptyset (&signal_mask);
                mpl::repiter{(sigaddset(&signal_mask,sign),1)...};
                pthread_sigmask(SIG_SETMASK, &signal_mask, NULL);
                pthread_attr_init(&attr);
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
                pthread_attr_setstacksize(&attr, stack_size);
                pthread_attr_setguardsize(&attr,0);
            }

            ~pthread_wrapper()
            {
                stop();
                pthread_attr_destroy(&attr);
            }

            void start()
            {
                if(!thread_handle_){
                    int res = pthread_create(&thread_handle_,&attr,start_routine,this);
                    if(res != 0){
                        string16_perror("pthread_create error");
                    }
                }
            }

            void stop()
            {
                stop_func_(var_);
                pthread_join(thread_handle_, NULL);
                thread_handle_ = 0;
            }

            const type_t &get()const
            {
                return var_;
            }

            type_t &get()
            {
                return var_;
            }

        private:

            void start_impl()
            {
                start_func_(var_);
            }

            static void *start_routine(void *d)
            {
                pthread_wrapper* wrap = static_cast<pthread_wrapper*>(d);
                if(wrap){
                    wrap->start_impl();
                }
                pthread_exit(NULL);
            }

            type_t var_;
            const start_func_type start_func_;
            const stop_func_type stop_func_;
            pthread_attr_t attr;
            pthread_t thread_handle_;
        };
    }
}
