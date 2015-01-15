#pragma once

#include <sys/time.h>

#include "common/types.h"

namespace common
{
    namespace time
    {
        struct timespec current_timespec();
        struct timeval current_timeval();
        time64_t current_mstime();

        struct timespec timeval2timespec(struct timeval* tv);
        struct timeval timespec2timeval(struct timespec* ts);

        time64_t timeval2mstime(struct timeval* tv);
        time64_t timespec2mstime(struct timespec* ts);

        struct timeval mstime2timeval(time64_t mst);
        struct timespec mstime2timespec(time64_t mst);
    }
}
