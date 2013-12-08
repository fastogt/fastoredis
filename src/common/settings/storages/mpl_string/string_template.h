#pragma once

#include "initializer_vector.h"
#include "mpl.h"
#include "common/file_system.h"

namespace common
{
    namespace mpl_template_string
    {
        template<typename init_vector>
        struct template_string
        {
            typedef init_vector init_vector_char;
            typedef typename mpl_vector::vector_to_string<init_vector_char>::type string_key;
            static const char *template_string_value(){return boost::mpl::c_str<string_key>::value;}
            static std::string template_string_value_path()
            {
                return common::file_system::prepare_path(boost::mpl::c_str<string_key>::value);
            }
        };
    }
}
