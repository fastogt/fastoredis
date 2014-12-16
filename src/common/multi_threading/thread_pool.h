#pragma once

#include <stdint.h>

namespace common
{
    namespace multi_threading
    {
        template<class impl_t>
        class thread_pool
        {
        public:
            typedef typename impl_t::task_type task_type;

            thread_pool()
                : pimpl_()
            {

            }

            void post(task_type task)
            {
                pimpl_.post(task);
            }

            void resize(uint16_t num, bool is_reload = false)
            {
                pimpl_.resize(num,is_reload);
            }

            void start(uint16_t num)
            {
                pimpl_.start(num);
            }

            void stop()
            {
                pimpl_.stop();
            }

            void restart()
            {
                pimpl_.restart();
            }

        private:
            thread_pool(const thread_pool&);
            thread_pool& operator=(const thread_pool&);

            impl_t pimpl_;
        };
    }
}
