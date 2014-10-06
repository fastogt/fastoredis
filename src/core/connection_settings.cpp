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
    const common::string16 defaultServerHost = UTEXT("localhost");
    const common::string16 defaultNameConnection = UTEXT("New Connection");
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const common::string16 &connectionName)
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

    common::string16 IConnectionSettingsBase::loggingPath() const
    {
        common::string16 logDir = SettingsManager::instance().loggingDirectory();
        return logDir + hash() + LOGGING_FILE_EXTENSION;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromString(const common::string16 &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            size_t len = val.size();

            uint8_t commaCount = 0;
            common::string16 elText;

            for(size_t i = 0; i < len; ++i ){
                char ch = val[i];
                if(ch == ','){
                    if(commaCount == 0){
                        int crT = elText[0] - 48;
                        switch(crT){
                            case REDIS:{
                                result = new RedisConnectionSettings(UTEXT(""));
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
                        result->setLoggingEnabled(common::convertFromString16<bool>(elText));
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

    common::string16 IConnectionSettingsBase::toString() const
    {
        common::string16 res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << logging_enabled_ << ',' << toCommandLine();
            res = common::convertToString16(str.str());
        }
        return res;
    }

    common::string16 IConnectionSettingsBase::hash() const
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

    void IConnectionSettingsBase::setConnectionName(const common::string16 &name)
    {        
        connectionName_ = name;
        using namespace common::utils;
        hash_ = common::convertToString16(hash::crc64(0, common::convertFromString16<common::buffer_type>(connectionName_)));
    }

    common::string16 IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    RedisConnectionSettings::RedisConnectionSettings(const common::string16 &connectionName, const redisConfig &info)
        :IConnectionSettingsBase(connectionName), info_(info)
    {

    }

    common::string16 RedisConnectionSettings::host() const
    {
        return info_.hostip;
    }

    void RedisConnectionSettings::setHost(const common::string16 &host)
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

    void RedisConnectionSettings::initFromCommandLine(const common::string16 &val)
    {
        info_ = common::convertFromString16<redisConfig>(val);
    }

    common::string16 RedisConnectionSettings::toCommandLine() const
    {
        common::string16 result = common::convertToString16(info_);
        return result;
    }

    void RedisConnectionSettings::setCommandLine(const common::string16 &line)
    {
        info_ = common::convertFromString16<redisConfig>(line);
    }

    common::string16 RedisConnectionSettings::commandLine() const
    {
        return common::convertToString16(info_);
    }

    common::string16 RedisConnectionSettings::fullAddress() const
    {
        common::net::hostAndPort h(host(), port());
        return common::convertToString16(h);
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
