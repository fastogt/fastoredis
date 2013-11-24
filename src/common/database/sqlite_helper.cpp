#include "sqlite_helper.h"
#include "sqlite3.h"
#include "common/logger.h"
namespace utils
{
    namespace delete_wrappers
    {
        template<>
        void default_delete<sqlite3>::operator ()(sqlite3 *ptr)const
        {
            sqlite3_close(ptr);
        }
        template<>
        void default_delete<sqlite3_stmt>::operator ()(sqlite3_stmt *ptr)const
        {
            sqlite3_finalize(ptr);
        }
    }
}
namespace database
{
    namespace sqlite_helper
    {
        struct sqlite::pimpl
        {
            database_ptr<sqlite3> connection;
        };
        struct sqlite_result_set : public iresult_set
        {
            typedef iresult_set base_class;
            explicit sqlite_result_set(idatabase *const db,const unicode_string &request):base_class(db,request)
            {
                sqlite3_stmt* inter = nullptr;
                sqlite *par = (sqlite*)(parent());
                sqlite3_prepare_v2((sqlite3*)par->connection(), request.c_str(), -1, &inter, 0);
                result_set.reset(inter);
                data_ = get_data();
            }
            const unicode_char* get_column_text(size_t column)const
            {
                const char* result = (const unicode_char*)sqlite3_column_text(result_set.get(), column);
                return result?result:UTEXT("n/a");
            }
            int get_column_int(size_t column)const
            {
                return sqlite3_column_int(result_set.get(), column);
            }
            container_rows_t get_data()const override
            {
                container_rows_t result;
                if(result_set)
                {
                    ssize_t column_count_ = sqlite3_column_count(result_set.get());
                    while (sqlite3_step(result_set.get())==SQLITE_ROW)
                    {
                        row_t r = smart_ptr::make_shared<row>(column_count_);
                        for(int i=0;i<column_count_;++i)
                        {
                            r->push_back(get_column_text(i));
                        }
                        result.push_back(r);
                    }
                }
                return result;
            }
            virtual void reset_impl()override
            {
                if(parent())
                {
                    sqlite3_stmt* inter = nullptr;
                    sqlite *par = (sqlite*)(parent());
                    sqlite3_prepare_v2((sqlite3*)par->connection(), request().c_str(), -1, &inter, 0);
                    result_set.reset(inter);
                }
            }
            database_ptr<sqlite3_stmt> result_set;
        };
        sqlite::sqlite(const connection_info& con_info):base_class(con_info),impl_(new pimpl)
        {

        }
        void *const sqlite::connection()const
        {
            return impl_->connection.get();
        }
        bool sqlite::connect_impl()
        {
            sqlite3 *inter=nullptr;
            sqlite3_open_v2(connection_info_.host_.c_str(), &inter,SQLITE_OPEN_READONLY,nullptr );
            impl_->connection.reset(inter);
            bool result = sqlite3_errcode(impl_->connection.get()) == SQLITE_OK;
            if(!result)
            {
                DEBUG_MSG_FORMAT<WARNING,128>("sqlite connect error: %s",sqlite3_errmsg(impl_->connection.get()));
            }
            return result;
        }
        void sqlite::disconnect_impl()
        {
            impl_->connection.reset();
        }
        idatabase::smart_shared_ptr_result_set_t sqlite::execute_request_impl(const unicode_string &request)
        {
            idatabase::smart_shared_ptr_result_set_t result;
            if(impl_->connection)
            {
                result.reset(new sqlite_result_set(this,request));
            }
            return result;
        }
        sqlite::~sqlite(){}
    }
}
