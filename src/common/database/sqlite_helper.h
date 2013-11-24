#ifndef COMMON_DATABASE_SQLITE_HELPER
#define COMMON_DATABASE_SQLITE_HELPER
#include <memory>
#include "common_helper.h"

namespace database
{
    namespace sqlite_helper
    {       
       struct sqlite
               : public idatabase
       {
           typedef idatabase base_class;
           explicit sqlite(const connection_info& con_info);
           void*const connection()const;
           ~sqlite();
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
