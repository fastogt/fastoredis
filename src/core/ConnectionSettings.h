#pragma once

#include <string>

#include <boost/intrusive_ptr.hpp>

#include "common/boost_extension.hpp"

namespace fastoredis
{
    class IConnectionSettingsBase
            : public common::boost_extension::intrusive_ptr_base<IConnectionSettingsBase>
    {
    public:
        enum connectionTypes
        {
            REDIS
        };

        virtual std::string fullAdress() const = 0;

        std::string connectionName() const;
        void setConnectionName(const std::string &name);

        virtual connectionTypes connectionType() const = 0;

    protected:
        IConnectionSettingsBase(const std::string &connectionName);
        std::string connectionName_;
    };

    typedef boost::intrusive_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

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
        virtual connectionTypes connectionType() const;

        config info() const;
        void setInfo(const config& info);
    private:
        config info_;
    };

    typedef boost::intrusive_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;

    namespace details
    {
        std::string toStdString(IConnectionSettingsBase *setting);
        IConnectionSettingsBase *fromStdString(const std::string &val);
    }
}
