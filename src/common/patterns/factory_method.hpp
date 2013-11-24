#pragma once
#include <vector>
#include <algorithm>
#include "common/smart_ptr.hpp"
#include "common/macros.h"
namespace common{
namespace patterns
{
    template<typename type_t>
    struct factory_method
    {
        typedef smart_ptr::shared_ptr<typename type_t::base_class> smart_shared_ptr_type;
        typedef type_t element_type;
        static const unicode_char* object_name;
        template<typename predicate_t>
        smart_shared_ptr_type get_item_by_predicate(predicate_t p) const
        {
            smart_shared_ptr_type result;
            auto it = std::find_if(pool.begin(),pool.end(),p);
            if(it!=pool.end())
            {
                result =*it;
            }
            return result;
        }
        template<typename ... targs_t>
        smart_shared_ptr_type create_object(targs_t ...args)
        {
            smart_shared_ptr_type result(new type_t(args...));
            if(result)
            {
                pool.push_back(result);
            }
            return result;
        }
        void remove_object(const smart_shared_ptr_type& t)
        {
            if(t)
            {
                pool.erase(std::remove(pool.begin(),pool.end(),t),pool.end());
            }
        }
        std::vector<smart_shared_ptr_type> pool;
    };
}
}
