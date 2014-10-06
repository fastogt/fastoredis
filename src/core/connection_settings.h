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
        common::string16 hash() const;

        common::string16 loggingPath() const;

        virtual common::string16 commandLine() const = 0;
        virtual void setCommandLine(const common::string16 &line) = 0;

        virtual common::string16 fullAddress() const = 0;

        virtual common::string16 host() const = 0;
        virtual void setHost(const common::string16 &host) = 0;
        virtual int port() const = 0;
        virtual void setPort(int port) = 0;

        common::string16 connectionName() const;
        void setConnectionName(const common::string16 &name);

        virtual connectionTypes connectionType() const;
        static IConnectionSettingsBase *fromString(const common::string16 &val);
        common::string16 toString() const;

        virtual IConnectionSettingsBase* clone () const = 0;

        bool loggingEnabled() const;
        void setLoggingEnabled(bool isLogging);

    protected:
        virtual common::string16 toCommandLine() const = 0;
        virtual void initFromCommandLine(const common::string16 &val) = 0;
        IConnectionSettingsBase(const common::string16 &connectionName);

    private:
        common::string16 connectionName_;
        common::string16 hash_;
        bool logging_enabled_;
    };

    typedef boost::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        RedisConnectionSettings(const common::string16 &connectionName, const redisConfig &info = redisConfig());

        virtual common::string16 commandLine() const;
        virtual void setCommandLine(const common::string16 &line);

        virtual common::string16 fullAddress() const;

        virtual common::string16 host() const;
        virtual void setHost(const common::string16 &host);
        virtual int port() const;
        virtual void setPort(int port);

        virtual connectionTypes connectionType() const;

        redisConfig info() const;
        void setInfo(const redisConfig& info);

        virtual IConnectionSettingsBase* clone () const;

    private:
        virtual common::string16 toCommandLine() const;
        virtual void initFromCommandLine(const common::string16 &val);
        redisConfig info_;
    };

    typedef boost::shared_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;
}
