#pragma once

/**/

#include "common/settings/storages/mpl_string/string_template.h"

namespace common
{
    namespace storages
    {
        namespace memory
        {
            struct memory_storage
            {
                typedef memory_storage load_struct;
                static const char *path_to_save()
                {
                    return NULL;
                }
                template<typename fusion_t>
                static bool load(fusion_t &fuc)
                {
                    return true;
                }
                template<typename fusion_t>
                static bool save(const fusion_t &fuc)
                {
                    return true;
                }
            };
        }
    }
}
