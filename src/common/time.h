#pragma once

#include "common/types.h"

namespace common
{
    namespace time
    {
        struct timespec current_timespec();
        struct timeval current_timeval();
        uint64_t current_mstime(void);

        struct timespec timeval2timespec(struct timeval* tv);
        struct timeval timespec2timeval(struct timespec* ts);

        uint64_t timeval2mstime(struct timeval* tv);
        uint64_t timespec2mstime(struct timespec* ts);

        struct timeval mstime2timeval(int64_t mst);
        struct timespec mstime2timespec(int64_t mst);
    }
}
