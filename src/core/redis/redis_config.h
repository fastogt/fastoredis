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
        common::string16 hostip;
        int hostport;
        common::string16 hostsocket;
        int repeat;
        int interval;
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
        common::string16 rdb_filename;
        int bigkeys;
        int stdinarg; /* get last arg from stdin. (-x option) */
        common::string16 auth;
        int output; /* output mode, see OUTPUT_* defines */
        common::string16 mb_delim;
        common::char16 prompt[128];
        common::string16 eval;
    };
}


namespace common
{
    string16 convertToString16(const fastoredis::redisConfig &conf);
}
