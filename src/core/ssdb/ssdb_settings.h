#pragma once

#include "core/connection_settings.h"

#include "core/ssdb/ssdb_config.h"

namespace fastoredis
{
    class SsdbConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        SsdbConnectionSettings(const std::string& connectionName);

        virtual std::string commandLine() const;
        virtual void setCommandLine(const std::string& line);

        virtual std::string host() const;
        virtual int port() const;
        virtual void setPort(int port);

        ssdbConfig info() const;
        void setInfo(const ssdbConfig &info);

        virtual IConnectionSettingsBase* clone() const;

    private:
        virtual std::string toCommandLine() const;
        virtual void initFromCommandLine(const std::string& val);
        ssdbConfig info_;
    };
}
