#include "oracle_helper.h"
#include <occi.h>
#include "common/logger.h"
namespace database
{
    namespace oracle_helper
    {
        using namespace oracle::occi;
        struct oracle_impl::pimpl
        {
            database_ptr<Connection> connection;
        };

        struct oracle_result_set
                : public iresult_set
        {
            typedef iresult_set base_class;
            oracle_result_set(idatabase *const db,const unicode_string &request,Connection *con):base_class(db,request)
            {
                shmt.reset(con->createStatement(request.c_str()));
                if(shmt)
                {
                    result_set.reset(shmt->executeQuery());
                }
            }
            container_rows_t get_data()const override
            {
                container_rows_t result;
                if(result_set)
                {
                    std::vector<MetaData> rec = result_set->getColumnListMetaData();
                    ssize_t column_count_ = rec.size();
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
                return result_set->isNull(column+1) ? UTEXT("n/a") :result_set->getString(column+1).c_str();
            }
            int get_column_int(size_t column)const
            {
                return result_set->isNull(column+1) ? -1 :result_set->getInt(column+1);
            }
            virtual void reset_impl()override
            {
                if(shmt)
                {
                    result_set.reset(shmt->executeQuery());
                }
            }
        private:
            database_ptr<ResultSet> result_set;
            database_ptr<Statement> shmt;
        };

        typedef utils::raii::scoped_alloc_dealloc_const<Environment*,&Environment::terminateEnvironment> scoped_oracle_env;
        Environment *get_enviroment()
        {
           typedef Environment *(*createEnv_t)(Environment::Mode,
                                               void *ctxp,
                                               void *(*malocfp)(void *ctxp, size_t size),
                                               void *(*ralocfp)(void *ctxp, void *memptr,size_t newsize),
                                               void (*mfreefp)(void *ctxp, void *memptr));
           static scoped_oracle_env env(static_cast<createEnv_t>(&Environment::createEnvironment),Environment::DEFAULT,nullptr,nullptr,nullptr,nullptr);
           return env.value_;
        }
        oracle_impl::oracle_impl(const connection_info& con_info):base_class(con_info),impl_(new pimpl)
        {

        }
        bool oracle_impl::connect_impl()
        {
            try
            {
                unicode_char message[256]={0};
                unicode_sprintf(message,"%s:%d/%s",connection_info_.host_.c_str(),connection_info_.port_,connection_info_.table_.c_str());//Connection Timeout=60;
                impl_->connection.reset(get_enviroment()->createConnection(connection_info_.user_,connection_info_.password_,message));
            }
            catch (const SQLException& ex)
            {
                DEBUG_MSG_FORMAT<WARNING,128>("oracle connect error: %s",ex.getMessage().c_str());
            }
            return impl_->connection!=nullptr;
        }
        void oracle_impl::disconnect_impl()
        {
            impl_->connection.reset();
        }
        idatabase::smart_shared_ptr_result_set_t oracle_impl::execute_request_impl(const unicode_string &request)
        {
            idatabase::smart_shared_ptr_result_set_t result;
            if(impl_->connection)
            {
                try
                {
                    result.reset(new oracle_result_set(this,request,impl_->connection.get()));
                }
                catch (const SQLException& ex)
                {
                    DEBUG_MSG_FORMAT<WARNING,128>("oracle execute_request error: %s",ex.getMessage().c_str());
                }
            }
            return result;
        }
        oracle_impl::~oracle_impl(){}
    }
   /* namespace detail
    {
            template<>
            ssize_t reader<oracle_impl_helper::oracle_impl>::get_row_count(const fwd_result_set_type &res)
            {
                ssize_t row_count = 0 ;
                OCIStmt *stmthp;
                OCIError *errhp;
                auto rc = OCIAttrGet ( stmthp, OCI_HTYPE_STMT, &row_count, 0, OCI_ATTR_ROW_COUNT,errhp );
                return row_count;
            }
    }*/
}
namespace utils
{
    namespace delete_wrappers
    {
        template<>
        void default_delete<oracle::occi::Connection>::operator ()(oracle::occi::Connection *ptr)const
        {
            if(::database::oracle_helper::get_enviroment()&&ptr)
            {
                ::database::oracle_helper::get_enviroment()->terminateConnection(ptr);
            }
        }
        template<>
        void default_delete<oracle::occi::Statement>::operator ()(oracle::occi::Statement *ptr)const
        {
            if(ptr)
            {
               oracle::occi::Connection *con = ptr->getConnection();
               if(con)
               {
                   con->terminateStatement(ptr);
               }
            }
        }
        template<>
        void default_delete<oracle::occi::ResultSet>::operator ()(oracle::occi::ResultSet *ptr)const
        {
            if(ptr)
            {
               oracle::occi::Statement *shmt = ptr->getStatement();
               if(shmt)
               {
                   shmt->closeResultSet(ptr);
               }
            }
        }
        //getConnection
    }
}
