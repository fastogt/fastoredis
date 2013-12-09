#include "core/ConnectionSettings.h"

#include <boost/lexical_cast.hpp>
#include "common/macros.h"
#include "common/net.h"

namespace
{
    const unsigned port = 27017;
    const char *defaultServerHost = "localhost";
    const char *defaultNameConnection = "New Connection";
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const std::string &connectionName)
        : connectionName_(connectionName)
    {

    }

    connectionTypes IConnectionSettingsBase::connectionType() const
    {
        return detail::badConnectionType();
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromStdString(const std::string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            int crT = boost::lexical_cast<int>(val[0]);
            switch(crT)
            {
            case REDIS:
            {
                result = new RedisConnectionSettings("");
                size_t len = val.size();
                for(size_t i = 2; i < len; ++i ){
                    char ch = val[i];
                    if(ch == ','){
                        std::string name = val.substr(2,i-2);
                        result->setConnectionName(name);
                        std::string line = val.substr(i+1);
                        result->initFromCommandLine(line);
                        break;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
        return result;
    }

    std::string IConnectionSettingsBase::toString() const
    {
        std::string res;
        connectionTypes crT = connectionType();
        if(crT != detail::badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << toCommandLine();
            res = str.str();
        }
        return res;
    }

    IConnectionSettingsBase::~IConnectionSettingsBase()
    {

    }

    void IConnectionSettingsBase::setConnectionName(const std::string &name)
    {
        connectionName_ = name;
    }

    std::string IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName, const redisConfig &info)
        :base_class(connectionName), info_(info)
    {

    }

    std::string RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    void RedisConnectionSettings::setHost(const std::string &host)
    {
        info_.hostip = host;
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
        info_ = rcFromStdString(val);
    }

    std::string RedisConnectionSettings::toCommandLine() const
    {
        std::string result = toStdString(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const std::string &line)
    {
        info_ = rcFromStdString(line);
    }

    std::string RedisConnectionSettings::commandLine() const
    {
        return toStdString(info_);
    }

    std::string RedisConnectionSettings::fullAddress() const
    {
        common::net::hostAndPort h(host(), port());
        return common::net::toStdString(h);
    }

    redisConfig RedisConnectionSettings::info() const
    {
        return info_;
    }

    void RedisConnectionSettings::setInfo(const redisConfig &info)
    {
        info_ =  info;
    }

    connectionTypes RedisConnectionSettings::connectionType() const
    {
        return REDIS;
    }

    IConnectionSettingsBase *RedisConnectionSettings::clone() const
    {
        RedisConnectionSettings *red = new RedisConnectionSettings(*this);
        return red;
    }
}
