#pragma once

/**/

#include "common/settings/storages/mpl_string/initializer_vector.h"
#include "common/settings/storages/mpl_string/mpl.h"
#include "common/file_system.h"

namespace common
{
    namespace mpl_template_string
    {
        template<typename init_vector>
        struct template_string
        {
            typedef init_vector init_vector_char;
            typedef typename init_vector::value_type value_type;
            typedef typename init_vector::string_type string_value;
            typedef typename mpl_vector::vector_to_string<typename init_vector_char::type>::type string_key;

            static const value_type* template_string_value()
            {
                return boost::mpl::c_str<string_key>::value;
            }

            static string_value template_string_value_path()
            {
                return common::file_system::prepare_path(boost::mpl::c_str<string_key>::value);
            }
        };
    }
}
