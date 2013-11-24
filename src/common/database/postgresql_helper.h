#ifndef POSTGRESQL_HELPER
#define POSTGRESQL_HELPER
#include <memory>
#include "common_helper.h"

namespace database
{
    namespace postgresql_helper
    {
       struct postgresql: public idatabase
       {
           typedef idatabase base_class;
           explicit postgresql(const connection_info& con_info);
           void*const connection()const;
           ~postgresql();
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
