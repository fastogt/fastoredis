#pragma once

/**/

namespace common
{
    namespace multi_threading
    {
        template<class impl_t>
        class thread_pool
        {
        public:
            typedef typename impl_t::task_type task_type;

            thread_pool():pimpl_()
            {

            }

            thread_pool(const thread_pool&) = delete;
            thread_pool& operator=(const thread_pool&) = delete;

            void post(task_type task)
            {
                pimpl_.post(task);
            }

            void resize(size_t num,bool is_reload=false)
            {
                pimpl_.resize(num,is_reload);
            }

            void start(size_t num)
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
            impl_t pimpl_;
        };
    }
}
