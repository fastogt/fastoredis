#pragma once
#ifdef BOOST_SUPPORT_ENABLED
#include <boost/intrusive_ptr.hpp>
#else
#include <memory>
#endif
namespace common
{
namespace smart_ptr
{
#ifdef BOOST_SUPPORT_ENABLED
    template<typename type>
    using shared_ptr = boost::intrusive_ptr<type>;
#else
    template<typename type>
    using shared_ptr = std::shared_ptr<type>;
#endif
    template<typename type_t,typename... args_t>
    inline shared_ptr<type_t> make_shared(args_t ...args)
    {
        return shared_ptr<type_t> (new type_t(args...));
    }
}
}
