#include "core/connection_settings.h"

#include <QObject>

#include "core/settings_manager.h"

#include "common/utils.h"
#include "common/net/net.h"
#include "common/logger.h"

#define LOGGING_FILE_EXTENSION ".red"

namespace
{
    const uint16_t port = 27017;
    const std::string defaultServerHost = "localhost";
    const std::string defaultNameConnection = "New Connection";
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const std::string &connectionName)
        : connectionName_(), hash_(), logging_enabled_(false), sshInfo_()
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

    std::string IConnectionSettingsBase::loggingPath() const
    {
        std::string logDir = SettingsManager::instance().loggingDirectory();
        return logDir + hash() + LOGGING_FILE_EXTENSION;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromString(const std::string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            size_t len = val.size();

            uint8_t commaCount = 0;
            std::string elText;

            for(size_t i = 0; i < len; ++i ){
                char ch = val[i];
                if(ch == ','){
                    if(commaCount == 0){
                        int crT = elText[0] - 48;
                        switch(crT){
                            case REDIS:{
                                result = new RedisConnectionSettings("", redisConfig());
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
                        result->setLoggingEnabled(common::convertFromString<uint8_t>(elText));
                    }
                    else if(commaCount == 3){
                        result->initFromCommandLine(elText);
                        result->setSshInfo(SSHInfo(val.substr(i+1)));
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

    std::string IConnectionSettingsBase::toString() const
    {
        std::string res;
        connectionTypes crT = connectionType();
        if(crT != badConnectionType()){
            std::stringstream str;
            str << crT << ',' << connectionName() << ',' << logging_enabled_ << ',' << toCommandLine() << ',' << sshInfo_.toString();
            res = str.str();
        }
        return res;
    }

    std::string IConnectionSettingsBase::hash() const
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

    SSHInfo IConnectionSettingsBase::sshInfo() const
    {
        return sshInfo_;
    }

    void IConnectionSettingsBase::setSshInfo(const SSHInfo& info)
    {
        sshInfo_ = info;
    }

    void IConnectionSettingsBase::setConnectionName(const std::string& name)
    {        
        connectionName_ = name;
        using namespace common::utils;
        common::buffer_type bcon = common::convertFromString<common::buffer_type>(connectionName_);
        uint64_t v = hash::crc64(0, bcon);
        hash_ = common::convertToString(v);
    }

    std::string IConnectionSettingsBase::connectionName() const
    {
        return connectionName_;
    }

    QString useHelpText(connectionTypes type)
    {
        if(type == DBUNKNOWN){
            return QString();
        }
        else if(type == REDIS){
            return QObject::tr("-h host -p port");
        }

        return QString();
    }

    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName, const redisConfig &info)
        :IConnectionSettingsBase(connectionName), info_(info)
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

    std::string RedisConnectionSettings::fullAddress() const
    {
        common::net::hostAndPort h(host(), port());
        return common::convertToString(h);
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
