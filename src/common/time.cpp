#include "common/time.h"

namespace common
{
    namespace time
    {
        struct timespec current_timespec()
        {
            struct timespec timeToWait;
            struct timeval now;
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec;
            timeToWait.tv_nsec = now.tv_usec * 1000;
            return timeToWait;
        }

        struct timeval current_timeval()
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return tv;
        }

        time64_t current_mstime()
        {
            struct timeval tv = current_timeval();
            return timeval2mstime(&tv);
        }

        struct timespec timeval2timespec(struct timeval* tv)
        {
            if(!tv){
                return timespec();
            }

            struct timespec ts;
            ts.tv_sec = tv->tv_sec;
            ts.tv_nsec = tv->tv_usec * 1000;
            return ts;
        }

        struct timeval timespec2timeval(struct timespec* ts)
        {
            if(!ts){
                return timeval();
            }
            struct timeval tv;
            tv.tv_sec = ts->tv_sec;
            tv.tv_usec = (ts->tv_nsec / 1000);
            return tv;
        }

        time64_t timeval2mstime(struct timeval* tv)
        {
            if(!tv){
                return 0;
            }

            time64_t mst = ((time64_t)tv->tv_sec * 1000);
            mst += tv->tv_usec/1000;
            return mst;
        }

        time64_t timespec2mstime(struct timespec* ts)
        {
            if(!ts){
                return 0;
            }

            uint64_t mst = ((time64_t)ts->tv_sec * 1000);
            mst += ts->tv_nsec/1000000;

            return mst;
        }

        struct timeval mstime2timeval(time64_t mst)
        {
            if(mst < 0){
                return timeval();
            }

            timeval tv;
            tv.tv_sec = mst/1000;
            tv.tv_usec = (mst % 1000) * 1000;
            return tv;
        }

        struct timespec mstime2timespec(time64_t mst)
        {
            if(mst < 0){
                return timespec();
            }

            timespec ts;
            ts.tv_sec = mst/1000;
            ts.tv_nsec = (mst % 1000) * 1000000;
            return ts;
        }
    }
}
