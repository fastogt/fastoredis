#pragma once

#include "common/types.h"

#include "common/convert2string.h"

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

    template < typename cast_to_t, typename cast_from_t >
    inline cast_to_t union_cast( cast_from_t _value )
    {
        union Union{ cast_from_t _from; cast_to_t _to; } unionCast;
        unionCast._from = _value;
        return unionCast._to;
    }

    template<typename type_t>
    struct variant_defualter
    {
        template<typename const_t>
        explicit variant_defualter(const_t def, typename boost::enable_if<detail::is_same_or_convertible<const_t, type_t> >::type* v=0) :
            value_(def)
        {

        }

        struct not_supported_type;
        explicit variant_defualter(not_supported_type* v=0);
        type_t operator()()const
        {
            return func_ ? func_():value_;
        }
        type_t value_;
        boost::function<type_t()> func_;
    };

    template<typename To, typename From>
    inline To lexical_cast(From val)
    {
        return val;
    }

    template<typename To, typename From>
    inline typename boost::enable_if< detail::is_same_or_convertible<To,string16> >::type lexical_cast(From val)
    {
        return convertToString16(val);
    }

    template<typename To, typename From>
    inline typename boost::enable_if< detail::is_same_or_convertible<string16,From> >::type lexical_cast(From val)
    {
        return convertFromString16<To>(val);
    }

    template<typename convert_to,typename convert_from>
    struct defualt_converter
    {
        convert_to operator()(const convert_from &src)const
        {
            return lexical_cast<convert_to>(src);
        }
    };

    template<typename convert_to,typename convert_from>
    inline convert_to make_conversion(convert_from from)
    {
        return defualt_converter<convert_to,convert_from>()(from);
    }
}
