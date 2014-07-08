#include "common/file_parsers/parser_ini.h"

#include <fstream>
#include <set>

#ifdef BOOST_ENABLED
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#endif

namespace common
{
    namespace file_parsers
    {
        namespace ini
        {
            bool parse_ini(const unicode_char* path, std::map<unicode_string,unicode_string> &ref)
            {
                 bool result = false;
                 std::ifstream config(path);
                 if(config)
                 {
                     std::set<unicode_string> options;
#ifdef BOOST_ENABLED
                     options.insert(UTEXT("*"));
                     for (boost::program_options::detail::basic_config_file_iterator<unicode_char>  i(config, options), e ; i != e; ++i)
                     {
                          ref[i->string_key] = i->value[0];
                     }
#else
#pragma message("implement parsing")
#endif
                     result = true;
                     config.close();
                 }
                 return result;
            }
        }
    }
}
