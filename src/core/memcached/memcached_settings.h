#pragma once

#include "core/connection_settings.h"

#include "core/memcached/memcached_config.h"

namespace fastoredis
{
    class MemcachedConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        MemcachedConnectionSettings(const std::string& connectionName);

        virtual std::string commandLine() const;
        virtual void setCommandLine(const std::string& line);

        virtual std::string host() const;
        virtual int port() const;
        virtual void setPort(int port);

        memcachedConfig info() const;
        void setInfo(const memcachedConfig& info);

        virtual IConnectionSettings* clone() const;

    private:
        virtual std::string toCommandLine() const;
        virtual void initFromCommandLine(const std::string& val);
        memcachedConfig info_;
    };
}
