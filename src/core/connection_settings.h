#pragma once

#include <boost/shared_ptr.hpp>

#include "core/connection_types.h"
#include "core/redis/redis_config.h"

namespace fastoredis
{
    class IConnectionSettingsBase
    {
    public:
        virtual ~IConnectionSettingsBase();
        common::unicode_string hash() const;

        virtual common::unicode_string commandLine() const = 0;
        virtual void setCommandLine(const common::unicode_string &line) = 0;

        virtual common::unicode_string fullAddress() const = 0;

        virtual common::unicode_string host() const = 0;
        virtual void setHost(const common::unicode_string &host) = 0;
        virtual int port() const = 0;
        virtual void setPort(int port) = 0;

        common::unicode_string connectionName() const;
        void setConnectionName(const common::unicode_string &name);

        virtual connectionTypes connectionType() const;
        static IConnectionSettingsBase *fromString(const common::unicode_string &val);
        common::unicode_string toString() const;

        virtual IConnectionSettingsBase* clone () const = 0;

        bool loggingEnabled() const;
        void setLoggingEnabled(bool isLogging);

    protected:
        virtual common::unicode_string toCommandLine() const = 0;
        virtual void initFromCommandLine(const common::unicode_string &val) = 0;
        IConnectionSettingsBase(const common::unicode_string &connectionName);

    private:
        common::unicode_string connectionName_;
        common::unicode_string hash_;
        bool logging_enabled_;
    };

    typedef boost::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        RedisConnectionSettings(const common::unicode_string &connectionName, const redisConfig &info = redisConfig());

        virtual common::unicode_string commandLine() const;
        virtual void setCommandLine(const common::unicode_string &line);

        virtual common::unicode_string fullAddress() const;

        virtual common::unicode_string host() const;
        virtual void setHost(const common::unicode_string &host);
        virtual int port() const;
        virtual void setPort(int port);

        virtual connectionTypes connectionType() const;

        redisConfig info() const;
        void setInfo(const redisConfig& info);

        virtual IConnectionSettingsBase* clone () const;

    private:
        virtual common::unicode_string toCommandLine() const;
        virtual void initFromCommandLine(const common::unicode_string &val);
        redisConfig info_;
    };

    typedef boost::shared_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;
}
