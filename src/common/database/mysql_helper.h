#ifndef COMMON_DATABASE_MYSQL_HELPER_H
#define COMMON_DATABASE_MYSQL_HELPER_H
#include <memory>
#include "common_helper.h"
namespace database
{
    namespace mysql_helper
    {
        struct mysql: public idatabase
        {
            typedef idatabase base_class;
            explicit mysql(const connection_info& con_info);
            ~mysql();
        private:
            virtual bool connect_impl() override;
            virtual void disconnect_impl() override;
            virtual idatabase::smart_shared_ptr_result_set_t execute_request_impl(const unicode_string &request) override;
            struct pimpl;
            const std::unique_ptr<pimpl> impl_;
        };
    }
}
#endif
