#pragma once

/**/

#include <vector>

#include "common/convert2string.h"

#define REDIS_CLI_DEFAULT_PIPE_TIMEOUT 30 /* seconds */
#define OUTPUT_STANDARD 0
#define OUTPUT_RAW 1
#define OUTPUT_CSV 2

namespace fastoredis
{
    struct redisConfig
    {
        redisConfig();
        ~redisConfig();

        char *hostip;
        int hostport;
        char *hostsocket;
        long repeat;
        long interval;
        int dbnum;
        int interactive;
        int shutdown;
        int monitor_mode;
        int pubsub_mode;
        int latency_mode;
        int latency_history;
        int cluster_mode;
        int cluster_reissue_command;
        int slave_mode;
        int pipe_mode;
        int pipe_timeout;
        int getrdb_mode;
        int stat_mode;
        int scan_mode;
        int intrinsic_latency_mode;
        int intrinsic_latency_duration;
        char *pattern;
        char *rdb_filename;
        int bigkeys;
        char *auth;
        char *eval;
        int last_cmd_type;
    };
}


namespace common
{
    std::string convertToString(const fastoredis::redisConfig &conf);
}
