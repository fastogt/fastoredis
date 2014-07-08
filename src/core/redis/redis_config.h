#pragma once

/**/

#include <vector>

#include "common/convert2string.h"

#define REDIS_DEFAULT_PIPE_TIMEOUT 30 /* seconds */

namespace fastoredis
{
    struct redisConfig {
        redisConfig();
        void parseOptions(const std::vector<std::string> &argv);
        common::unicode_string hostip;
        int hostport;
        common::unicode_string hostsocket;
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
        common::unicode_string rdb_filename;
        int bigkeys;
        int stdinarg; /* get last arg from stdin. (-x option) */
        common::unicode_string auth;
        int output; /* output mode, see OUTPUT_* defines */
        common::unicode_string mb_delim;
        common::unicode_char prompt[128];
        common::unicode_string eval;
    };
}


namespace common
{
    unicode_string convert2string(const fastoredis::redisConfig &conf);
}
