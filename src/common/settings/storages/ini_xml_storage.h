#pragma once

#include <istream>
#include <iostream>

#include "common/settings/storages/mpl_string/string_template.h"

#include <boost/property_tree/ptree.hpp>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace common
{
    namespace storages
    {
        namespace detail
        {
            template<class Ptree>
            struct load_funct
            {
                load_funct(const Ptree& set)
                    : set_(set)
                {

                }

                template<typename type_t>
                void operator()(type_t& item) const
                {
                    typedef typename type_t::value_type value_t;
                    value_t result = set_.template get<value_t>(item.key(), item.value());
                    item.load_value(result);
                }

            private:
                const Ptree& set_;
            };

            template<class Ptree>
            struct save_funct
            {
                save_funct(Ptree& set)
                    : set_(set)
                {

                }

                template<typename type_t>
                void operator()(const type_t& item) const
                {
                    const typename type_t::value_type &value = item.value();
                    set_.put(item.key(), value);
                }

            private:
                Ptree& set_;
            };
        }

        enum storage_type
        {
            Xml,
            Ini
        };

        template<storage_type type>
        struct storage_traits
        {
            template<class Ptree>
            static void write_impl(std::ofstream &stream, const Ptree &pt);
            template<class Ptree>
            static void read_impl(std::istream &stream, Ptree &pt);
        };

        template<> template<class Ptree>
        void storage_traits<Xml>::write_impl(std::ofstream &output, const Ptree &pt)
        {
            boost::property_tree::write_xml(output, pt);
        }

        template<> template<class Ptree>
        void storage_traits<Xml>::read_impl(std::istream &input, Ptree &pt)
        {
            boost::property_tree::read_xml(input, pt);
        }

        template<> template<class Ptree>
        void storage_traits<Ini>::write_impl(std::ofstream &output, const Ptree &pt)
        {
            boost::property_tree::ini_parser::write_ini(output, pt);
        }

        template<> template<class Ptree>
        void storage_traits<Ini>::read_impl(std::istream &input, Ptree &pt)
        {
            boost::property_tree::ini_parser::read_ini(input, pt);
        }

        template<storage_type type, typename init_vector, bool createDir = false>
        struct storage_base
        {
            static const storage_type value = type;
            typedef storage_traits<value> storage_traits_type;

            typedef init_vector init_vector_char;
            typedef typename init_vector::value_type value_type;
            typedef typename init_vector::string_type string_value;

            typedef string_value key_type;
            typedef std::string data_type;

            typedef boost::property_tree::basic_ptree<key_type, data_type> ptree;
            typedef detail::load_funct<ptree> load_struct;
            typedef detail::save_funct<ptree> save_struct;

            static string_value path_to_save()
            {
                typedef mpl_template_string::template_string<init_vector> path_to_save_t;
                return path_to_save_t::template_string_value_path();
            }

            template<typename fusion_t>
            static bool load(fusion_t &fuc);
            template<typename fusion_t>
            static bool save(const fusion_t &fuc);
        };

        template<storage_type type, typename init_vector, bool createDir>template<typename fusion_t>
        bool storage_base<type, init_vector, createDir>::save(const fusion_t &fuc)
        {
            const string_value path = path_to_save();
            if(createDir){
                std::string rdir = file_system::get_dir_path(path);
                if(file_system::is_directory(rdir) != SUCCESS){
                    bool isok = file_system::create_directory(rdir, true);
                    DCHECK(isok);
                }
            }
            const value_type *path_c = path.c_str();
            std::ofstream output(path_c);
            if(output.is_open()){
                ptree pt;
                ptree &settings = pt.add("settings", "");
                boost::fusion::for_each(fuc, save_struct(settings));
                storage_traits_type::write_impl(output, pt);
                output.close();
            }
            return true;
        }

        template<storage_type type, typename init_vector, bool createDir>template<typename fusion_t>
        bool storage_base<type, init_vector, createDir>::load(fusion_t &fuc)
        {
            const string_value path = path_to_save();
            if(createDir){
                std::string rdir = file_system::get_dir_path(path);
                if(file_system::is_directory(rdir) != SUCCESS){
                    bool isok = file_system::create_directory(rdir, true);
                    DCHECK(isok);
                }
            }
            const value_type *path_c = path.c_str();
            std::ifstream input(path_c);
            if(!input.is_open()){
                input.open(path_c, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
            }
            if(input.is_open()){
                ptree pt;
                try
                {
                    storage_traits_type::read_impl(input, pt);
                    const ptree& settings = pt.get_child("settings");
                    boost::fusion::for_each(fuc, load_struct(settings));
                }
                catch(const boost::property_tree::ptree_bad_data& error)
                {
                    std::cout << error.what() << std::endl;
                }
                catch(const boost::property_tree::ptree_bad_path& error)
                {
                    std::cout << error.what() << std::endl;
                }
                catch(const std::exception& error)
                {
                    std::cout << error.what() << std::endl;
                }
                input.close();
            }
            return true;
        }

        namespace ini
        {
            template<typename init_vector, bool createDir = false>
            struct ini_storage
                    : public storage_base<Ini, init_vector, createDir>
            {

            };
        }

        namespace xml
        {
            template<typename init_vector, bool createDir = false>
            struct ini_storage
                    : public storage_base<Xml, init_vector, createDir>
            {

            };
        }
    }
}
