#pragma once

/**/

#include "common/settings/storages/storages_def.h"
#include "common/lexical_cast.h"

namespace common
{
    namespace settings
    {
        template<typename type_t, typename init_vector, class where>
        struct setting
        {
            typedef type_t value_type;
            typedef where storage_type;

            friend typename where::load_struct;

            template<typename const_t>
            setting(const const_t &def)
                :default_value(def),value_(defualt()),is_load_(false),is_changed_(false)
            {
            }

            const typename init_vector::value_type *key() const
            {
                typedef mpl_template_string::template_string<init_vector> key_t;
                return key_t::template_string_value();
            }

            value_type defualt() const
            {
                return default_value();
            }

            bool set_value(value_type value)
            {
                bool result = value_ != value;
                if(result){
                    value_ = value;
                    is_changed_ = true;
                }
                return result;
            }

            value_type value() const
            {
                return value_;
            }

            bool is_changed()const
            {
                return is_changed_;
            }

            bool is_load()const
            {
                return is_load_;
            }

        private:
            bool load_value(value_type value)
            {
                is_load_ = true;
                return set_value(value);
            }
            const common::variant_defualter<value_type> default_value;
            value_type value_;
            bool is_load_;
            bool is_changed_;
        };
    }
}

#define BEGIN_DECL_ARRAY(NAME,TYPE,STORAGE)			namespace genereted_settings{ typedef common::settings::setting<TYPE,GEN_STRING_SEQ(NAME),STORAGE> setting_##NAME; }
#define BEGIN_DECL_TYPLE(NAME,TYPE,STORAGE)			namespace genereted_settings{ typedef common::settings::setting<TYPE,GEN_STRING_TYPLE(NAME),STORAGE > setting_##NAME;}

