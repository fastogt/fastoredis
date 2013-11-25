#pragma once

#include <string>

namespace fastoredis
{
    class IConnectionSettingsBase
    {
    public:
        virtual std::string fullAdress() const = 0;
        std::string connectionName() const;
        void setConnectionName(const std::string &name);
    protected:
        IConnectionSettingsBase(const std::string &connectionName);
        std::string connectionName_;
    };

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
        typedef IConnectionSettingsBase base_class;
        typedef char *sds;
        struct config {
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
            char *rdb_filename;
            int bigkeys;
            int stdinarg; /* get last arg from stdin. (-x option) */
            char *auth;
            int output; /* output mode, see OUTPUT_* defines */
            sds mb_delim;
            char prompt[128];
            char *eval;
        };

    public:
        RedisConnectionSettings(const std::string &connectionName, const config &info);
        virtual std::string fullAdress() const;

        config info() const;
        void setInfo(const config& info);
    private:
        config info_;
    };
}
