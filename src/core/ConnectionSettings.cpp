#include "core/ConnectionSettings.h"

namespace
{
    const unsigned port = 27017;
    const char *defaultServerHost = "localhost";
    const char *defaultNameConnection = "New Connection";
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const std::string &connectionName)
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

    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName, const config &info)
        :base_class(connectionName), info_(info)
    {

    }

    std::string RedisConnectionSettings::fullAdress() const
    {
        return std::string();
    }

    RedisConnectionSettings::config RedisConnectionSettings::info() const
    {
        return info_;
    }

    void RedisConnectionSettings::setInfo(const config& info)
    {
        info_ =  info;
    }

    IConnectionSettingsBase::connectionTypes RedisConnectionSettings::connectionType() const
    {
        return REDIS;
    }

    namespace details
    {
        std::string toStdString(IConnectionSettingsBase *setting)
        {
            std::string result;
            if(setting){

            }
            return result;
        }

        IConnectionSettingsBase *fromStdString(const std::string &val)
        {
            return NULL;
        }
    }
}
