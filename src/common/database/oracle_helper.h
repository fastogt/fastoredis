#ifndef COMMON_DATABASE_ORACLE_HELPER_H
#define COMMON_DATABASE_ORACLE_HELPER_H
#include <memory>
#include "common_helper.h"
namespace database
{
    namespace oracle_helper
    {
        struct oracle_impl : public idatabase
        {
            typedef idatabase base_class;
            explicit oracle_impl(const connection_info& con_info);
            ~oracle_impl();
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
