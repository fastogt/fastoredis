#include "core/memcached/memcached_settings.h"

namespace fastoredis
{
    MemcachedConnectionSettings::MemcachedConnectionSettings(const std::string& connectionName)
        : IConnectionSettingsBase(connectionName, MEMCACHED), info_()
    {

    }

    std::string MemcachedConnectionSettings::commandLine() const
    {
        return common::convertToString(info_);
    }

    void MemcachedConnectionSettings::setCommandLine(const std::string& line)
    {
        info_ = common::convertFromString<memcachedConfig>(line);
    }

    std::string MemcachedConnectionSettings::host() const
    {
        return info_.hostip_;
    }

    int MemcachedConnectionSettings::port() const
    {
        return info_.hostport_;
    }

    void MemcachedConnectionSettings::setPort(int port)
    {
        info_.hostport_ = port;
    }

    memcachedConfig MemcachedConnectionSettings::info() const
    {
        return info_;
    }

    void MemcachedConnectionSettings::setInfo(const memcachedConfig& info)
    {
        info_ = info;
    }

    IConnectionSettings *MemcachedConnectionSettings::clone() const
    {
        MemcachedConnectionSettings *red = new MemcachedConnectionSettings(*this);
        return red;
    }

    std::string MemcachedConnectionSettings::toCommandLine() const
    {
        std::string result = common::convertToString(info_);
        return result;
    }

    void MemcachedConnectionSettings::initFromCommandLine(const std::string& val)
    {
        info_ = common::convertFromString<memcachedConfig>(val);
    }
}
