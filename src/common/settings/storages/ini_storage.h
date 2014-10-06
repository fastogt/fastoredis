#pragma once

/**/

#include <istream>
#include <iostream>

#include "common/file_system.h"

#include "common/settings/storages/mpl_string/string_template.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace common
{
    namespace storages
    {
        namespace ini
        {
            namespace detail
            {
                struct load_funct
                {
                    load_funct(const boost::property_tree::ptree& set):set_(set){}
                    template<typename type_t>
                    void operator()(type_t& item) const
                    {
                        using boost::property_tree::ptree;
                        typedef typename type_t::value_type value_type;
                        value_type result = item.value();
                        boost::property_tree::ptree::const_assoc_iterator it = set_.find( item.key() );
                        if( it != set_.not_found() ){
                            ptree::value_type const& v = (*it);
                            boost::property_tree::ptree node = v.second;
                            std::string str = node.get<std::string>("");
                            result = node.get<value_type>("");
                        }
                        item.load_value(result);
                    }
                private:
                    const boost::property_tree::ptree& set_;
                };
                struct save_funct
                {
                    save_funct(boost::property_tree::ptree& set):set_(set){}
                    template<typename type_t>
                    void operator()(const type_t& item) const
                    {
                        using boost::property_tree::ptree;
                        const typename type_t::value_type &value = item.value();
                        set_.put(item.key(),value);
                    }
                private:
                    boost::property_tree::ptree& set_;
                };
            }
            template<typename init_vector, bool createDir = false>
            struct ini_storage
            {
                typedef detail::load_funct load_struct;
                static std::string path_to_save()
                {
                    typedef mpl_template_string::template_string<init_vector> path_to_save_t;
                    return path_to_save_t::template_string_value_path();
                }

                template<typename fusion_t>
                static bool load(fusion_t &fuc);
                template<typename fusion_t>
                static bool save(const fusion_t &fuc);
            };

            template<typename init_vector, bool createDir>template<typename fusion_t>
            bool ini_storage<init_vector, createDir>::save(const fusion_t &fuc)
            {
                std::string path = path_to_save();
                if(createDir){
                    std::string rdir = file_system::get_dir_path(path);
                    if(file_system::is_directory(rdir) != SUCCESS){
                        file_system::create_directory(rdir, true);
                    }
                }
                const char *path_c = path.c_str();
                std::ofstream output(path_c);
                if(output.is_open()){
                    boost::property_tree::ptree pt;
                    boost::property_tree::ptree &settings= pt.add("settings","");
                    boost::fusion::for_each(fuc, detail::save_funct(settings));
                    boost::property_tree::ini_parser::write_ini( output, pt );
                    output.close();
                }
                return true;
            }

            template<typename init_vector, bool createDir>template<typename fusion_t>
            bool ini_storage<init_vector, createDir>::load(fusion_t &fuc)
            {
                std::string path = path_to_save();
                if(createDir){
                    std::string rdir = file_system::get_dir_path(path);
                    if(file_system::is_directory(rdir) != SUCCESS){
                        file_system::create_directory(rdir, true);
                    }
                }
                const char *path_c = path.c_str();
                std::ifstream input(path_c);
                if(!input.is_open()){
                    input.open(path_c, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
                }
                if(input.is_open()){
                    boost::property_tree::ptree pt;
                    try
                    {
                        boost::property_tree::ini_parser::read_ini(input, pt);
                        const boost::property_tree::ptree& settings = pt.get_child("settings");
                        boost::fusion::for_each(fuc, detail::load_funct(settings));
                    }
                    catch (const boost::property_tree::ptree_bad_data& error)
                    {
                        std::cout << error.what() << std::endl;
                    }
                    catch (const boost::property_tree::ptree_bad_path& error)
                    {
                        std::cout << error.what() << std::endl;
                    }
                    input.close();
                }
                return true;
            }
        }
    }
}
