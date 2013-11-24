#include "common_helper.h"
#include <string.h>
namespace
{
const unicode_char eol = UTEXT('\n');
}
namespace database
{
    bool operator <(const idatabase &lhs,const idatabase &rhs)
    {
        return lhs.connection_information()<rhs.connection_information();
    }
    bool operator <(const idatabase::smart_shared_ptr_result_set_t &lhs,const idatabase::smart_shared_ptr_result_set_t &rhs)
    {
        return lhs->request() < rhs->request()|| (*lhs->parent()) < (*rhs->parent());
    }
    idatabase::idatabase(const connection_info &connection_info)
        :connection_info_(connection_info),is_coonected_(false)
    {

    }
    idatabase::~idatabase()
    {

    }
    void idatabase::release_result_set(iresult_set* res)
    {

    }
    bool idatabase::is_connected()const
    {
        return is_coonected_;
    }
    bool idatabase::connect()
    {
        if(!is_coonected_)
        {
            is_coonected_ = connect_impl();
        }
        return is_coonected_;
    }
    void idatabase::disconnect()
    {
        if(is_coonected_)
        {
            disconnect_impl();
        }
    }
    idatabase::smart_shared_ptr_result_set_t idatabase::execute_request(const unicode_string &request)
    {
        smart_shared_ptr_result_set_t result;
        if(is_coonected_)
        {
            auto founded = pool_result_map_.find(request);
            if(founded==pool_result_map_.end())
            {
                result = execute_request_impl(request);
                if(result)
                {
                    pool_result_map_[request] = result;
                }
            }
            else
            {
                result = (*founded).second;
            }
        }
        return result;
    }
    void idatabase::clear()
    {
        #pragma message("TODO: Check for zombie")
        pool_result_map_.clear();
    }
    row::row(uint8_t column_count)
        :separators_indexes_((size_t*)calloc(column_count,sizeof(size_t))),data_((unicode_char*)calloc(column_count*20,sizeof(unicode_char)))
        ,lenght_(0),column_count_(column_count),current_(0)
    {
    }
    row::~row()
    {
        free(separators_indexes_);
        free(data_);
    }
    unicode_string row::get_column(uint32_t n) const
    {
        unicode_string result;
        if(current_>n)
        {
            u_char start = separators_indexes_[n];
            u_char end = separators_indexes_[n+1];
            result = unicode_string(data_+start,data_+end-1);
        }
        return result;
    }
    void row::push_back(const unicode_char* val)
    {
        if(val&&current_!=column_count())
        {
            size_t size_ = unicode_strlen(val);
            memcpy(data_+lenght_,val,size_);
            lenght_+=size_;
            if(++current_!=column_count())
            {
                data_[lenght_] = separator;
            }
            else
            {
                data_[lenght_] = eol;
            }
            separators_indexes_[current_]=++lenght_;
        }
    }
    iresult_set::iresult_set(const idatabase * const db, const unicode_string &request):parent_(db),request_(request)
    {
    }
    void iresult_set::reset()
    {
        reset_impl();
        data_.clear();
    }

    const iresult_set::container_rows_t &iresult_set::read()
    {
        if(data_.empty())
        {
            data_ = get_data();
        }
        return data_;
    }
    const idatabase *const iresult_set::parent() const
    {
        return parent_;
    }
    const unicode_string& iresult_set::request() const
    {
        return request_;
    }
    iresult_set::~iresult_set(){}
    database_info::database_info(){}
    bool database_info::is_valid()const
    {
        return !database_type_name_.empty()&&con_info_.is_valid();
    }
    database_info::database_info(const unicode_string &database_type_name,const database::connection_info &con_info)
        :database_type_name_(database_type_name),con_info_(con_info)
    {

    }
    database_info make_database_info(const unicode_string &database_connection_string, const unicode_char separator)
    {
        database_info result;
        size_t pos = database_connection_string.find_first_of(separator);
        if(pos!=unicode_string::npos)
        {
            #pragma message("TODO: refactor plz")
            unicode_string database_type_name = database_connection_string.substr(0,pos);
            unicode_string connection_string = database_connection_string.substr(pos+1);
            if(!database_type_name.empty()&&!connection_string.empty())
            {
                result = database_info(database_type_name, database::connection_info(connection_string) );
            }
        }
        return result;
    }
}
