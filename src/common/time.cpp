#include "common/time.h"

#include <sys/time.h>

namespace common
{
    namespace time
    {
        long long mstime(void) {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            long long mst = ((long long)tv.tv_sec)*1000;
            mst += tv.tv_usec/1000;
            return mst;
        }

        std::string msTimeToString(long long ms)
        {
            return "0";
        }
    }
}
