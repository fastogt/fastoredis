#include "connection_info.h"
#include "common/mpl.hpp"
namespace database
{
    connection_info::connection_info():host_(),port_(0),user_(),password_(),table_(){}
    connection_info::connection_info(const unicode_string &host,const unicode_string &port,const unicode_string &user,const unicode_string &password,const unicode_string &table)
        :host_(host),port_(utils::converter::make_conversion<size_t>(port)),user_(user),password_(password),table_(table)
    {
    }
    bool connection_info::is_valid()const
    {
        return !host_.empty()&&port_&&!user_.empty()&&!password_.empty()&&!table_.empty();
    }
    connection_info::connection_info(const unicode_string &connection_string,const unicode_char separator):host_(),port_(0),user_(),password_(),table_()
    {
        size_t member=0;
        const auto helper = mpl::get_member::detail::make_get_member_helper<connection_info,unicode_string,size_t,unicode_string,unicode_string,unicode_string>(this);
        for(unsigned i=0;i<connection_string.size();++i)
        {
            if(connection_string[i]!=separator)
            {
                void *mem = mpl::get_member::get_member(member,helper);
                if(mem)
                {
                    if(mem==&port_)
                    {
                        port_=port_*10+connection_string[i]-'0';
                    }
                    else
                    {
                          unicode_string *str = static_cast<unicode_string*>(mem);
                          *str+=connection_string[i];
                    }
                }
            }
            else
            {
                ++member;
                if(member>5)
                {
                    break;
                }
            }
        }
    }
}
