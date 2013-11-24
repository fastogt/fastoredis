#ifndef COMMON_DATABASE_COMMON_HELPER_H
#define COMMON_DATABASE_COMMON_HELPER_H
#include <map>
#include <vector>
#include "connection_info.h"
#include "common/smart_ptr.hpp"
#include "common/boost_extension.hpp"
#include "common/utils.h"
namespace database
{
    class iresult_set;
    template<typename type>
    using database_ptr = std::unique_ptr<type,utils::delete_wrappers::default_delete<type> >;

    class idatabase
#ifdef BOOST_SUPPORT_ENABLED
            :public boost_extension::intrusive_ptr_base<idatabase>
#endif
    {
    public:
        typedef smart_ptr::shared_ptr<iresult_set> smart_shared_ptr_result_set_t;
        explicit idatabase(const connection_info &connection_info);
        bool is_connected()const;
        bool connect();
        void disconnect();
        void release_result_set(iresult_set* res);
        virtual ~idatabase();
        smart_shared_ptr_result_set_t execute_request(const unicode_string &request);
        const connection_info& connection_information() const
        {
            return connection_info_;
        }
        void clear();
    protected:
        virtual bool connect_impl()=0;
        virtual void disconnect_impl()=0;
        virtual smart_shared_ptr_result_set_t execute_request_impl(const unicode_string &request)=0;
        const connection_info connection_info_;
        bool is_coonected_;
        std::map<unicode_string,smart_shared_ptr_result_set_t> pool_result_map_;
    };
    bool operator <(const idatabase &lhs,const idatabase &rhs);
    class row
#ifdef BOOST_SUPPORT_ENABLED
            : public boost_extension::intrusive_ptr_base<row>
#endif
    {
    public:
        enum {separator = UTEXT(',')};
        explicit row(uint8_t column_count);
        void push_back(const unicode_char* val);
        uint8_t column_count()const
        {
            return column_count_;
        }
        size_t lenght()const
        {
            return lenght_;
        }
        unicode_string get_column(uint32_t n) const;
        const unicode_char *const get_data() const
        {
            return data_;
        }
        ~row();
    private:        
        row(const row&);
        row& operator=(const row&);
        size_t *const separators_indexes_;
        unicode_char *const data_;
        size_t lenght_;
        const uint32_t column_count_;
        uint32_t current_;
    };

    class iresult_set
#ifdef BOOST_SUPPORT_ENABLED
            : public boost_extension::intrusive_ptr_base<iresult_set>
#endif
    {
    public:
        typedef smart_ptr::shared_ptr<row> row_t;
        typedef std::vector<row_t> container_rows_t;
        explicit iresult_set(const idatabase *const db,const unicode_string &request);
        virtual ~iresult_set();
        void reset();
        const container_rows_t &read();
        const idatabase *const parent() const;
        const unicode_string& request() const;
    protected:
        container_rows_t data_;
    private:
        const idatabase *const  parent_;
        const unicode_string request_;
        virtual void reset_impl()=0;
        virtual container_rows_t get_data()const=0;
    };
    bool operator <(const idatabase::smart_shared_ptr_result_set_t &lhs,const idatabase::smart_shared_ptr_result_set_t &rhs);

    struct database_info
    {
        database_info();
        database_info(const unicode_string &database_type_name,const database::connection_info &con_info);
        bool is_valid()const;
        unicode_string database_type_name_;
        database::connection_info con_info_;
    };
    database_info make_database_info(const unicode_string &database_connection_string,const unicode_char separator = UTEXT(','));
    inline bool operator==(const database_info& x,const database_info& y)
    {
        return x.database_type_name_ == y.database_type_name_&&x.con_info_ == y.con_info_;
    }
    inline bool operator!=(const database_info& x,const database_info& y)
    {
        return !operator ==(x,y);
    }
}
#endif
