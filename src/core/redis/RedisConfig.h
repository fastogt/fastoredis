#pragma once

#include <vector>
#include <string>

#define REDIS_DEFAULT_PIPE_TIMEOUT 30 /* seconds */

namespace fastoredis
{
    struct redisConfig {
        redisConfig();
        void parseOptions(const std::vector<std::string> &argv);
        std::string hostip;
        int hostport;
        std::string hostsocket;
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
        std::string rdb_filename;
        int bigkeys;
        int stdinarg; /* get last arg from stdin. (-x option) */
        std::string auth;
        int output; /* output mode, see OUTPUT_* defines */
        std::string mb_delim;
        char prompt[128];
        std::string eval;
    };

    std::string toStdString(const redisConfig &conf);
    redisConfig rcFromStdString(const std::string &line);
}
