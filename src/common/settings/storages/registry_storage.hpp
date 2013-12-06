#pragma once

#define REGISTER_SUPPORT

#include "mpl_string/string_template.h"
#include <boost/fusion/include/for_each.hpp>
#include <Windows.h>
namespace common
{
namespace storages
{
	namespace windows
	{
		namespace registry
		{
			namespace detail
			{
				typedef std::pair<HKEY,std::string> key_path_t;			
				inline HKEY translate_to_hkey(const std::string &path)
				{
					static const key_path_t mas_hkey_string []=
					{
						key_path_t(HKEY_CLASSES_ROOT,"HKEY_CLASSES_ROOT"),
						key_path_t(HKEY_CURRENT_CONFIG,"HKEY_CURRENT_CONFIG"),
						key_path_t(HKEY_CURRENT_USER,"HKEY_CURRENT_USER"),
						key_path_t(HKEY_LOCAL_MACHINE,"HKEY_LOCAL_MACHINE"),
						key_path_t(HKEY_USERS,"HKEY_USERS")
					};

					HKEY result;
                    int count = sizeof(mas_hkey_string)/sizeof(*mas_hkey_string);
                    for (int i = 0; i < count; ++i)
					{
                        const key_path_t item = mas_hkey_string[i];
						if(item.second.compare(path)==0)
						{
							result = item.first;
							break;
						}
					}
					return result;
				}
				inline key_path_t translate_path_to_hkey(const std::string &path)
				{
					key_path_t result;
					size_t pos(path.find_first_of("\\"));				
					if(pos != std::string::npos)
					{
						result.first = translate_to_hkey(path.substr (0,pos));
						result.second = path.c_str() + pos+1;
                        if(*result.second.rbegin() == '\\'){
                            result.second = result.second.substr(0, result.second.length()-2);
						}
					}
					return result;
				}
				template<typename type_t,typename enable = void>
				struct convert_traits
				{
					static const DWORD type_of_key = REG_SZ;
					typedef char bufer_type;
					static bool convert(const bufer_type *mass,type_t &val)
					{
						val = type_t(mass);
						return true;
					}
					static size_t get_lenght(const type_t &val)
					{
						return strlen(val);
					}
					static BYTE* convert_to_byte_ptr(const type_t &val)
					{
						return (LPBYTE)&*val;
					}
				};
				template<>
				struct convert_traits<bool>
				{ 
					static const DWORD type_of_key = REG_DWORD;
					typedef bool bufer_type;
					static bool convert(const bufer_type *mass,bool &val)
					{
						val=*mass;
						return true;
					}
					static size_t get_lenght(const bool val)
					{
						return sizeof(int);
					}
					static BYTE* convert_to_byte_ptr(const bool &val)
					{
						static int adr = 0;
						adr = val?1:0;
						return (BYTE*)&adr;
					}
				};
				template<typename type_t>
                struct convert_traits<type_t,typename boost::enable_if<boost::is_integral<type_t> >::type >
				{					
					static const DWORD type_of_key = REG_DWORD;
					typedef int bufer_type;
					static bool convert(const bufer_type *mass,type_t &val)
					{
						val=*mass;
						return true;
					}
					static size_t get_lenght(const type_t &val)
					{
						return sizeof(int);
					}
					static BYTE* convert_to_byte_ptr(const type_t &val)
					{
						return (LPBYTE)&val;
					}
				};
				template<>
				struct convert_traits<std::string,void>
				{					
					static const DWORD type_of_key = REG_SZ;
					typedef char bufer_type;
					static bool convert(const bufer_type *mass,std::string &val)
					{
						val=mass;
						return true;
					}
					static size_t get_lenght(const std::string &val)
					{
						return val.length();
					}
					static BYTE* convert_to_byte_ptr(const std::string &val)
					{
						return (LPBYTE)val.c_str();
					}
				};	
				struct load_funct
				{
					load_funct(const key_path_t &key):root_(key){}
					template<typename type_t>		
					void operator()(type_t& item) const
					{
						typedef typename type_t::value_type type_item;
						typedef convert_traits<type_item> converter_type;					
						HKEY hKey;					
						if(RegOpenKeyExA(root_.first,root_.second.c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
						{
							typename converter_type::bufer_type buf[256]={0};
							DWORD cbData=sizeof(buf);
							DWORD type_of_key  = converter_type::type_of_key;
							if(RegQueryValueExA(hKey, item.key(), NULL, &type_of_key,(LPBYTE)buf, &cbData) == ERROR_SUCCESS)
							{
								type_item tmp;
								bool is_convert = converter_type::convert(buf,tmp);
								if(is_convert)
								{
									item.load_value(tmp);
								}
							}						
							RegCloseKey(hKey);
						}					
					}
				private:
					const key_path_t root_;
				};
				struct save_funct
				{
					save_funct(const key_path_t &key):root_(key){}
					template<typename type_t>		
					void operator()(const type_t& item) const
					{
						if (item.is_changed())
						{
							typedef typename type_t::value_type type_item;
							typedef convert_traits<type_item> converter_type;					
							HKEY hKey;		
							SECURITY_ATTRIBUTES sec;
							sec.nLength = sizeof(sec);
							sec.lpSecurityDescriptor = 0;
							sec.bInheritHandle = true; 
							if(RegCreateKeyExA(root_.first,root_.second.c_str(), 0,NULL, REG_OPTION_VOLATILE, KEY_WRITE,&sec, &hKey,NULL) == ERROR_SUCCESS)
							{
								type_item buf= item.value();
								DWORD cbData=converter_type::get_lenght(buf);	
								RegSetValueExA(hKey, item.key(), 0, converter_type::type_of_key,converter_type::convert_to_byte_ptr(buf) , cbData);
								RegCloseKey(hKey);
							}	
						}	
					}
				private:
					const key_path_t root_;
				};
			}
			template<typename init_vector>
			struct registry_storage
			{
				typedef detail::load_funct load_struct;
				static const char *path_to_save()
				{
                    typedef mpl_template_string::template_string<init_vector> path_to_save_t;
					return path_to_save_t::template_string_value();
				}
				template<typename fusion_t>
				static bool load(fusion_t &fuc)
				{
					detail::key_path_t p = detail::translate_path_to_hkey(path_to_save());
					boost::fusion::for_each(fuc, detail::load_funct(p));
					return true;
				}
				template<typename fusion_t>
				static bool save(const fusion_t &fuc)
				{
					detail::key_path_t p = detail::translate_path_to_hkey(path_to_save());
					boost::fusion::for_each(fuc, detail::save_funct(p));
					return true;
				}
			};
		}
	}
}
}
