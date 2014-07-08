#include "core/connection_settings.h"

#include "common/macros.h"
#include <boost/lexical_cast.hpp>
#include "common/net.h"

namespace
{
    const unsigned port = 27017;
    const char *defaultServerHost = "localhost";
    const char *defaultNameConnection = "New Connection";
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const common::unicode_string &connectionName)
        : connectionName_(connectionName)
    {

    }

    connectionTypes IConnectionSettingsBase::connectionType() const
    {
        return badConnectionType();
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromStdString(const common::unicode_string &val)
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
                        common::unicode_string name = val.substr(2,i-2);
                        result->setConnectionName(name);
                        common::unicode_string line = val.substr(i+1);
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

    common::unicode_string IConnectionSettingsBase::toString() const
    {
        common::unicode_string res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << toCommandLine();
            res = str.str();
        }
        return res;
    }

    IConnectionSettingsBase::~IConnectionSettingsBase()
    {

    }

    void IConnectionSettingsBase::setConnectionName(const common::unicode_string &name)
    {
        connectionName_ = name;
    }

    common::unicode_string IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    RedisConnectionSettings::RedisConnectionSettings(const common::unicode_string &connectionName, const redisConfig &info)
        :IConnectionSettingsBase(connectionName), info_(info)
    {

    }

    common::unicode_string RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    void RedisConnectionSettings::setHost(const common::unicode_string &host)
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

    void RedisConnectionSettings::initFromCommandLine(const common::unicode_string &val)
    {
        info_ = common::convertfromString<redisConfig>(val);
    }

    common::unicode_string RedisConnectionSettings::toCommandLine() const
    {
        common::unicode_string result = common::convert2string(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const common::unicode_string &line)
    {
        info_ = common::convertfromString<redisConfig>(line);
    }

    common::unicode_string RedisConnectionSettings::commandLine() const
    {
        return common::convert2string(info_);
    }

    common::unicode_string RedisConnectionSettings::fullAddress() const
    {
        common::net::hostAndPort h(host(), port());
        return common::convert2string(h);
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
