#include "core/ConnectionSettings.h"

#include <stdio.h>
#include "common/utils.h"

namespace
{
    const unsigned port = 27017;
    const char *defaultServerHost = "localhost";
    const char *defaultNameConnection = "New Connection";
    const char *connnectionType[fastoredis::IConnectionSettingsBase::REDIS+1] = { "Unknown", "Redis" };
}

namespace fastoredis
{
    IConnectionSettingsBase::IConnectionSettingsBase(const std::string &connectionName)
        : connectionName_(connectionName)
    {

    }

    IConnectionSettingsBase::connectionTypes IConnectionSettingsBase::connectionType() const
    {
        return UNKNOWN;
    }

    IConnectionSettingsBase *IConnectionSettingsBase::fromStdString(const std::string &val)
    {
        IConnectionSettingsBase *result = NULL;
        if(!val.empty()){
            int crT = boost::lexical_cast<int>(val[0]);
            switch(crT)
            {
            case REDIS:
                result = new RedisConnectionSettings("");
                break;
            default:
                break;
            }
            if(result){
                size_t len = val.size();
                int comma = 0;
                int start = 0;
                for(size_t i=0; i < len; ++i ){
                    char ch = val[i];
                    if(ch == ','){
                        if(comma == 1){
                            std::string name = val.substr(start+1,i-start-1);
                            result->setConnectionName(name);
                        }
                        else if(comma == 2){
                            std::string host = val.substr(start+1,i-start-1);
                            result->setHost(detail::toHostAndPort(host));
                            result->initFromStringImpl(val.c_str()+i);
                            break;
                        }
                        comma++;
                        start = i;
                    }
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
            str << crT << ',' << connectionName() << ',' << detail::toStdString(host()) << ',';
            res+=str.str();
            res+=toStringImpl();
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

    RedisConnectionSettings::RedisConnectionSettings(const std::string &connectionName, const config &info)
        :base_class(connectionName), info_(info)
    {

    }

    hostAndPort RedisConnectionSettings::host() const
    {
        return info_.host;
    }

    void RedisConnectionSettings::setHost(const hostAndPort &host)
    {
        info_.host = host;
    }

    void RedisConnectionSettings::initFromStringImpl(const std::string &val)
    {

    }

    std::string RedisConnectionSettings::toStringImpl() const
    {
        std::string result;

        return result;
    }

    std::string RedisConnectionSettings::fullAdress() const
    {
        return detail::toStdString(info_.host);
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

    namespace detail
    {
        std::vector<std::string> supportedConnectionTypes()
        {
            return common::utils::enums::convertToVector(connnectionType);
        }

        std::string toStdString(IConnectionSettingsBase::connectionTypes t)
        {
            std::string result;
            int count = sizeof(connnectionType)/sizeof(*connnectionType);
            if(t < count){
                result = connnectionType[t];
            }
            return result;
        }

        IConnectionSettingsBase::connectionTypes toConnectionType(const std::string &text)
        {
            return common::utils::enums::findTypeInArray<IConnectionSettingsBase::connectionTypes>(connnectionType,text.c_str());
        }

        std::string toStdString(const hostAndPort &host)
        {
            unicode_char buff[512] = {0};
            const char* h = host.first.c_str();
            if(h){
                sprintf(buff,"%s:%u", h, host.second);
            }
            return buff;
        }

        hostAndPort toHostAndPort(const std::string &host)
        {
            hostAndPort res;
            size_t del = host.find_first_of(':');
            if(del != std::string::npos){
                res.first = host.substr(0, del);
                res.second = boost::lexical_cast<unsigned>(host.substr(del + 1));
            }
            return res;
        }
    }
}
