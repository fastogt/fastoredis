#pragma once

#include <vector>

#include "common/multi_threading/lockers.h"

namespace common
{
    namespace multi_threading
    {
        namespace thread_safe_containers
        {
            template <typename type_t>
            class vector
            {
            public:
                typedef std::vector<type_t> container_type;
                typedef typename container_type::value_type value_type;

                vector()
                    : mutex_(), array_items_()
                {
                }

                ~vector()
                {
                }

                void push_back(const value_type& val)
                {
                    unique_lock<mutex_t> lock(mutex_);
                    array_items_.push_back(val);
                }

                void pop_back()
                {
                    unique_lock<mutex_t> lock(mutex_);
                    array_items_.pop_back();
                }

                int size()const
                {
                    unique_lock<mutex_t> lock(mutex_);
                    return array_items_.size();
                }

                bool empty()
                {
                    unique_lock<mutex_t> lock(mutex_);
                    return array_items_.empty();
                }

            private:
                mutex_t mutex_;
                container_type array_items_;
            };
        }
    }
}
