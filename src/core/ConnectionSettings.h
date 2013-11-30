#pragma once

#include <boost/shared_ptr.hpp>
#include "common/boost_extension.hpp"
#include "core/ConnectionTypes.h"
#include "core/HostAndPort.h"

namespace fastoredis
{
    class IConnectionSettingsBase
    {
    public:

        virtual std::string fullAdress() const = 0;

        virtual hostAndPort host() const = 0;
        virtual void setHost(const hostAndPort &host) = 0;

        std::string connectionName() const;
        void setConnectionName(const std::string &name);

        virtual connectionTypes connectionType() const;
        static IConnectionSettingsBase *fromStdString(const std::string &val);
        std::string toString() const;

        virtual IConnectionSettingsBase* clone () const = 0;
        virtual ~IConnectionSettingsBase();
    protected:
        virtual std::string toStringImpl() const = 0;
        virtual void initFromStringImpl(const std::string &val) = 0;

        IConnectionSettingsBase(const std::string &connectionName);
        std::string connectionName_;
    };

    typedef boost::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
        typedef IConnectionSettingsBase base_class;
        typedef char *sds;
        struct config {
            hostAndPort host;
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
        RedisConnectionSettings(const std::string &connectionName, const config &info = config());

        virtual std::string fullAdress() const;

        virtual hostAndPort host() const;
        virtual void setHost(const hostAndPort &host);

        virtual connectionTypes connectionType() const;

        config info() const;
        void setInfo(const config& info);

        virtual IConnectionSettingsBase* clone () const;
    private:
        virtual std::string toStringImpl() const;
        virtual void initFromStringImpl(const std::string &val);
        config info_;
    };

    typedef boost::shared_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;
}
