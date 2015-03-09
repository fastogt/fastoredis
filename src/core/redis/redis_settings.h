#pragma once

#include "core/connection_settings.h"

#include "core/redis/redis_config.h"

namespace fastoredis
{
    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        RedisConnectionSettings(const std::string& connectionName);

        virtual std::string commandLine() const;
        virtual void setCommandLine(const std::string& line);

        virtual std::string host() const;
        virtual int port() const;
        virtual void setPort(int port);

        redisConfig info() const;
        void setInfo(const redisConfig& info);

        virtual IConnectionSettingsBase* clone() const;

    private:
        virtual std::string toCommandLine() const;
        virtual void initFromCommandLine(const std::string& val);
        redisConfig info_;
    };
}
