#include "mysql_helper.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "common/utils.h"
#include "common/logger.h"
namespace database
{
    namespace mysql_helper
    {
        struct mysql::pimpl
        {
            database_ptr<sql::Connection> connection;
        };
        struct mysql_result_set : public iresult_set
        {
            typedef iresult_set base_class;
            mysql_result_set(idatabase *const db,const unicode_string &request,sql::Connection *connection):base_class(db,request)
            {
                shmt.reset(connection->createStatement());
                if(shmt)
                {
                    sql::ResultSet* res=nullptr;
                    try
                    {
                        res = shmt->executeQuery(request.c_str());
                    }
                    catch(const std::exception& ex)
                    {
                        DEBUG_MSG_FORMAT<WARNING,128>("MySql result_set error: %s",ex.what());
                    }
                    if(res)
                    {
                        result_set.reset(res);
                    }
                }
            }
            base_class::container_rows_t get_data()const override
            {
                container_rows_t result;
                if(result_set)
                {
                    //row_count_ = result_set->rowsCount() - 1;
                    sql::ResultSetMetaData *md =  result_set->getMetaData();
                    ssize_t column_count_=  md->getColumnCount();
                    if(result_set)
                    {
                        while (result_set->next())
                        {
                            row_t r = smart_ptr::make_shared<row>(column_count_);
                            for(int i=0;i<column_count_;++i)
                            {
                                r->push_back(get_column_text(i));
                            }
                            result.push_back(r);
                        }
                    }
                }
                return result;
            }
            const unicode_char* get_column_text(size_t column)const
            {
                return result_set->isNull(column+1) ? UTEXT("n/a") : result_set->getString(column+1).c_str();
            }
            int get_column_int(size_t column)const
            {
                return result_set->isNull(column+1) ? -1 : result_set->getInt(column+1);
            }
            virtual void reset_impl()override
            {
                if(shmt)
                {
                    result_set.reset(shmt->executeQuery(request().c_str()));
                }
            }
            database_ptr<sql::Statement> shmt;
            database_ptr<sql::ResultSet> result_set;
        };
        mysql::mysql(const connection_info& con_info):base_class(con_info),impl_(new pimpl)
        {

        }
        bool mysql::connect_impl()
        {
            unicode_string host_and_port = connection_info_.host_;
            host_and_port+=UTEXT(":");
            host_and_port+=utils::converter::make_conversion<unicode_string>(connection_info_.port_);
            try
            {
                impl_->connection.reset(get_driver_instance()->connect(host_and_port, connection_info_.user_,connection_info_.password_ ));//connection timeout = 15
                impl_->connection->setSchema(connection_info_.table_);
            }
            catch(const std::exception& ex)
            {
                DEBUG_MSG_FORMAT<WARNING,128>("MySql connect error: %s",ex.what());
            }
            return impl_->connection.get()!=nullptr;
        }
        void mysql::disconnect_impl()
        {
            impl_->connection.reset();
        }
        idatabase::smart_shared_ptr_result_set_t mysql::execute_request_impl(const unicode_string &request)
        {
            idatabase::smart_shared_ptr_result_set_t result;
            if(impl_->connection)
            {
                result.reset(new mysql_result_set(this,request,impl_->connection.get()));
            }
            return result;
        }
        mysql::~mysql(){}
    }
}
