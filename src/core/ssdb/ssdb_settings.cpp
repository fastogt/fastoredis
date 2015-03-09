#include "core/ssdb/ssdb_settings.h"

namespace fastoredis
{
    SsdbConnectionSettings::SsdbConnectionSettings(const std::string& connectionName)
        : IConnectionSettingsBase(connectionName, SSDB), info_()
    {

    }

    std::string SsdbConnectionSettings::commandLine() const
    {
        return common::convertToString(info_);
    }

    void SsdbConnectionSettings::setCommandLine(const std::string& line)
    {
        info_ = common::convertFromString<ssdbConfig>(line);
    }

    std::string SsdbConnectionSettings::host() const
    {
        return info_.hostip;
    }

    int SsdbConnectionSettings::port() const
    {
        return info_.hostport;
    }

    void SsdbConnectionSettings::setPort(int port)
    {
        info_.hostport = port;
    }

    ssdbConfig SsdbConnectionSettings::info() const
    {
        return info_;
    }

    void SsdbConnectionSettings::setInfo(const ssdbConfig& info)
    {
        info_ = info;
    }

    IConnectionSettingsBase* SsdbConnectionSettings::clone() const
    {
        SsdbConnectionSettings *red = new SsdbConnectionSettings(*this);
        return red;
    }

    std::string SsdbConnectionSettings::toCommandLine() const
    {
        std::string result = common::convertToString(info_);
        return result;
    }

    void SsdbConnectionSettings::initFromCommandLine(const std::string& val)
    {
        info_ = common::convertFromString<ssdbConfig>(val);
    }

}
