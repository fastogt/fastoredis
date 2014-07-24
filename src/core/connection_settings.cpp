#include "core/connection_settings.h"

#include <sstream>

#include "core/settings_manager.h"

#include "common/utils.h"
#include "common/net.h"
#include "common/convert2string.h"

#define LOGGING_FILE_EXTENSION UTEXT(".red")

namespace
{
    const common::uint16_type port = 27017;
    const common::unicode_string defaultServerHost = UTEXT("localhost");
    const common::unicode_string defaultNameConnection = UTEXT("New Connection");
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const common::unicode_string &connectionName)
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

    common::unicode_string IConnectionSettingsBase::loggingPath() const
    {
        common::unicode_string logDir = SettingsManager::instance().loggingDirectory();
        return logDir + hash() + LOGGING_FILE_EXTENSION;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromString(const common::unicode_string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            size_t len = val.size();

            common::uint8_type commaCount = 0;
            common::unicode_string elText;

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

    common::unicode_string IConnectionSettingsBase::toString() const
    {
        common::unicode_string res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << logging_enabled_ << ',' << toCommandLine();
            res = str.str();
        }
        return res;
    }

    common::unicode_string IConnectionSettingsBase::hash() const
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

    void IConnectionSettingsBase::setConnectionName(const common::unicode_string &name)
    {        
        connectionName_ = name;
        using namespace common::utils;
        hash_ = common::convert2string(hash::crc64(0, common::convertfromString<common::buffer_type>(connectionName_)));
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
