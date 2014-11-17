#pragma once

#ifdef BOOST_ENABLED
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#endif

namespace common
{
    namespace detail
    {
        template<typename type_t,typename type_u>
        struct is_same_or_convertible
        {
            static const bool value = boost::is_same<type_t, type_u>::value || boost::is_convertible<type_t, type_u>::value;
        };
    }

    template <typename cast_to_t, typename cast_from_t>
    inline cast_to_t union_cast(cast_from_t value)
    {
        union Union{ cast_from_t _from; cast_to_t _to; } unionCast;
        unionCast._from = value;
        return unionCast._to;
    }

    template<typename type_t>
    struct variant_defualter
    {
        template<typename const_t>
        explicit variant_defualter(const_t def, typename boost::enable_if<detail::is_same_or_convertible<const_t, type_t> >::type* v = 0)
            : value_(def)
        {

        }

        type_t operator()() const
        {
            return func_ ? func_() : value_;
        }

        type_t value_;
        boost::function<type_t()> func_;
    };
}
