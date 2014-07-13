#pragma once

/**/

#include "common/types.h"

namespace common
{
    namespace time
    {
        struct timespec current_timespec();
        struct timeval current_timeval();
        long long current_mstime(void);

        struct timespec timeval2timespec(struct timeval* tv);
        struct timeval timespec2timeval(struct timespec* ts);

        long long timeval2mstime(struct timeval* tv);
        long long timespec2mstime(struct timespec* ts);

        struct timeval mstime2timeval(int64_type mst);
        struct timespec mstime2timespec(int64_type mst);

        unicode_string mstime2string(int64_type ms);

        time_t timegm(struct tm * time_tm);
    }
}
