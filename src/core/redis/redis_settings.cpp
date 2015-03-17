#include "core/redis/redis_settings.h"

#include "common/utils.h"

namespace fastoredis
{
    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName)
        : IConnectionSettingsBase(connectionName, REDIS), info_()
    {

    }

    std::string RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    void RedisConnectionSettings::setHost(const std::string& host)
    {
        info_.hostip = common::utils::strdupornull(host);
    }

    int RedisConnectionSettings::port() const
    {
        return info_.hostport;
    }

    void RedisConnectionSettings::setPort(int port)
    {
        info_.hostport = port;
    }

    void RedisConnectionSettings::initFromCommandLine(const std::string &val)
    {
        info_ = common::convertFromString<redisConfig>(val);
    }

    std::string RedisConnectionSettings::toCommandLine() const
    {
        std::string result = common::convertToString(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const std::string &line)
    {
        info_ = common::convertFromString<redisConfig>(line);
    }

    std::string RedisConnectionSettings::commandLine() const
    {
        return common::convertToString(info_);
    }

    redisConfig RedisConnectionSettings::info() const
    {
        return info_;
    }

    void RedisConnectionSettings::setInfo(const redisConfig &info)
    {
        info_ =  info;
    }

    IConnectionSettings *RedisConnectionSettings::clone() const
    {
        RedisConnectionSettings *red = new RedisConnectionSettings(*this);
        return red;
    }
}
