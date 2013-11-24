#include "postgresql_helper.h"
#include "libpq-fe.h"
#include "common/logger.h"
typedef struct pg_conn PGconn;
typedef struct pg_result PGresult;
namespace utils
{
    namespace delete_wrappers
    {
        template<>
        void default_delete<PGconn>::operator ()(PGconn *ptr)const
        {
            PQfinish(ptr);
        }
        template<>
        void default_delete<PGresult>::operator ()(PGresult *ptr)const
        {
            PQclear(ptr);
        }
    }
}

namespace database
{
    namespace postgresql_helper
    {
        struct postgresql::pimpl
        {
            database_ptr<PGconn> connection;
        };
        struct postgresql_result_set: public iresult_set
        {
            typedef iresult_set base_class;
            postgresql_result_set(idatabase *const db,const unicode_string &request):base_class(db,request)
            {
                postgresql *par = (postgresql *)(parent());
                if(par)
                {
                    PGresult * res = PQexec(static_cast<PGconn*>(par->connection()),request.c_str());
                    if(res)
                    {
                        result_set.reset(res);
                    }
                }
            }
            const unicode_char* get_column_text(size_t row,size_t column)const
            {
                int is_null = PQgetisnull(result_set.get(), row, column);
                return !is_null ? PQgetvalue(result_set.get(), row, column):UTEXT("n/a");
            }
            int get_column_int(size_t row,size_t column)const
            {
                int is_null = PQgetisnull(result_set.get(), row, column);
                return !is_null ? PQgetlength(result_set.get(), row, column):-1;
            }
            container_rows_t get_data()const override
            {
                container_rows_t result;
                if(result_set)
                {
                    const int row_count_ = PQntuples(result_set.get());
                    const int column_count_ = PQnfields(result_set.get());
                    for(int ro = 0;ro< row_count_;++ro)
                    {
                        row_t r = smart_ptr::make_shared<row>(column_count_);
                        for(int i=0;i<column_count_;++i)
                        {
                            r->push_back(get_column_text(ro,i));
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
                    postgresql *par = (postgresql *)(parent());
                    result_set.reset(PQexec(static_cast<PGconn*>(par->connection()),request().c_str()));
                }
            }
            database_ptr<PGresult> result_set;
        };
        postgresql::postgresql(const connection_info& con_info):base_class(con_info),impl_(new pimpl)
        {

        }        
        void *const postgresql::connection()const
        {
            return impl_->connection.get();
        }
        bool postgresql::connect_impl()
        {
            unicode_char message[256]={0};
            unicode_sprintf(message,"user=%s password=%s dbname=%s hostaddr=%s port=%d connect_timeout=30",connection_info_.user_.c_str(),
                            connection_info_.password_.c_str(),connection_info_.table_.c_str(),
                            connection_info_.host_.c_str(),connection_info_.port_);
            impl_->connection.reset(PQconnectdb(message));
            bool result = PQstatus(impl_->connection.get()) != CONNECTION_BAD;
            if(!result)
            {
                DEBUG_MSG_FORMAT<WARNING,256>("postgresql connect error: %s",PQerrorMessage(impl_->connection.get()));
            }
            return result;
        }
        void postgresql::disconnect_impl()
        {
            impl_->connection.reset();
        }
        idatabase::smart_shared_ptr_result_set_t postgresql::execute_request_impl(const unicode_string &request)
        {
            idatabase::smart_shared_ptr_result_set_t result;
            if(impl_->connection)
            {
                result.reset(new postgresql_result_set(this,request));
            }
            return result;
        }
        postgresql::~postgresql(){}
    }
}
