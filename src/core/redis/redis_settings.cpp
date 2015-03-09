#include "core/redis/redis_settings.h"

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

    IConnectionSettingsBase *RedisConnectionSettings::clone() const
    {
        RedisConnectionSettings *red = new RedisConnectionSettings(*this);
        return red;
    }
}
