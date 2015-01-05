#pragma once

#include <vector>

#include "common/convert2string.h"

#define REDIS_CLI_DEFAULT_PIPE_TIMEOUT 30 /* seconds */

namespace fastoredis
{
    struct memcachedConfig
    {
        memcachedConfig();
        memcachedConfig(const memcachedConfig& other);
        memcachedConfig& operator=(const memcachedConfig &other);

        ~memcachedConfig();

        char *hostip;
        int hostport;
        char *mb_delim;

    private:
        void copy(const memcachedConfig& other);
        void init();
    };
}

namespace common
{
    std::string convertToString(const fastoredis::memcachedConfig &conf);
}
