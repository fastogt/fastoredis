#include "core/connection_settings.h"

#include <sstream>

#include "core/settings_manager.h"

#include "common/utils.h"
#include "common/net.h"
#include "common/convert2string.h"

#define LOGGING_FILE_EXTENSION UTEXT(".red")

namespace
{
    const uint16_t port = 27017;
    const unicode_string defaultServerHost = UTEXT("localhost");
    const unicode_string defaultNameConnection = UTEXT("New Connection");
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const unicode_string &connectionName)
        : connectionName_(), hash_(), logging_enabled_(false)
    {
        setConnectionName(connectionName.empty() ? defaultNameConnection : connectionName);
    }

    IConnectionSettingsBase::~IConnectionSettingsBase()
    {

    }

    connectionTypes IConnectionSettingsBase::connectionType() const
    {
        return badConnectionType();
    }

    unicode_string IConnectionSettingsBase::loggingPath() const
    {
        unicode_string logDir = SettingsManager::instance().loggingDirectory();
        return logDir + hash() + LOGGING_FILE_EXTENSION;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromString(const unicode_string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            size_t len = val.size();

            uint8_t commaCount = 0;
            unicode_string elText;

            for(size_t i = 0; i < len; ++i ){
                char ch = val[i];
                if(ch == ','){
                    if(commaCount == 0){
                        int crT = elText[0] - 48;
                        switch(crT){
                            case REDIS:{
                                result = new RedisConnectionSettings("");
                                break;
                            }
                            default:{
                                NOTREACHED();
                                return result;
                            }
                        }
                    }
                    else if(commaCount == 1){
                        result->setConnectionName(elText);
                    }
                    else if(commaCount == 2){
                        result->setLoggingEnabled(common::convertfromString<bool>(elText));
                        result->initFromCommandLine(val.substr(i+1));
                        break;
                    }
                    commaCount++;
                    elText.clear();
                }
                else{
                   elText += ch;
                }
            }
        }
        return result;
    }

    unicode_string IConnectionSettingsBase::toString() const
    {
        unicode_string res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << logging_enabled_ << ',' << toCommandLine();
            res = str.str();
        }
        return res;
    }

    unicode_string IConnectionSettingsBase::hash() const
    {
        return hash_;
    }

    bool IConnectionSettingsBase::loggingEnabled() const
    {
        return logging_enabled_;
    }

    void IConnectionSettingsBase::setLoggingEnabled(bool isLogging)
    {
        logging_enabled_ = isLogging;
    }

    void IConnectionSettingsBase::setConnectionName(const unicode_string &name)
    {        
        connectionName_ = name;
        using namespace common::utils;
        hash_ = common::convert2string(hash::crc64(0, common::convertfromString<buffer_type>(connectionName_)));
    }

    unicode_string IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    RedisConnectionSettings::RedisConnectionSettings(const unicode_string &connectionName, const redisConfig &info)
        :IConnectionSettingsBase(connectionName), info_(info)
    {

    }

    unicode_string RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    void RedisConnectionSettings::setHost(const unicode_string &host)
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

    void RedisConnectionSettings::initFromCommandLine(const unicode_string &val)
    {
        info_ = common::convertfromString<redisConfig>(val);
    }

    unicode_string RedisConnectionSettings::toCommandLine() const
    {
        unicode_string result = common::convert2string(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const unicode_string &line)
    {
        info_ = common::convertfromString<redisConfig>(line);
    }

    unicode_string RedisConnectionSettings::commandLine() const
    {
        return common::convert2string(info_);
    }

    unicode_string RedisConnectionSettings::fullAddress() const
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
