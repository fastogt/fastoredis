#ifndef COMMON_DATABASE_CONNECTION_INFO_HPP
#define COMMON_DATABASE_CONNECTION_INFO_HPP
#include "common/macros.h"
namespace database
{
    struct connection_info
    {
        connection_info();
        connection_info(const unicode_string &host,const unicode_string &port,const unicode_string &user,const unicode_string &password,const unicode_string &table);
        explicit connection_info(const unicode_string &connection_string,const unicode_char separator = UTEXT(',') );
        bool is_valid()const;
        unicode_string host_;
        size_t port_;
        unicode_string user_;
        unicode_string password_;
        unicode_string table_;        
    };
    inline bool operator <(const connection_info &lhs,const connection_info &rhs)
    {
        return lhs.host_ < rhs.host_ ||  lhs.port_ < rhs.port_ || lhs.user_ < rhs.user_ || lhs.password_ < rhs.password_ || lhs.table_ < rhs.table_;
    }
    inline bool operator!=(const connection_info& lhs,const connection_info& rhs)
    {
        return lhs.host_ != rhs.host_ ||  lhs.port_ != rhs.port_ || lhs.user_ != rhs.user_ || lhs.password_ != rhs.password_ || lhs.table_ != rhs.table_;
    }
    inline bool operator==(const connection_info& lhs,const connection_info& rhs)
    {
        return lhs.host_ == rhs.host_ &&  lhs.port_ == rhs.port_ && lhs.user_ == rhs.user_ && lhs.password_ == rhs.password_ && lhs.table_ == rhs.table_;
    }
}
#endif
