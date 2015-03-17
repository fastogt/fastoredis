#include "core/ssdb/ssdb_settings.h"

#include "common/utils.h"

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
        return info_.hostip_;
    }

    void SsdbConnectionSettings::setHost(const std::string& host)
    {
        info_.hostip_ = common::utils::strdupornull(host);
    }

    int SsdbConnectionSettings::port() const
    {
        return info_.hostport_;
    }

    void SsdbConnectionSettings::setPort(int port)
    {
        info_.hostport_ = port;
    }

    ssdbConfig SsdbConnectionSettings::info() const
    {
        return info_;
    }

    void SsdbConnectionSettings::setInfo(const ssdbConfig& info)
    {
        info_ = info;
    }

    IConnectionSettings* SsdbConnectionSettings::clone() const
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
