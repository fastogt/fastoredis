#pragma once

#include <boost/shared_ptr.hpp>
#include "common/boost_extension.hpp"
#include "core/ConnectionTypes.h"
#include "core/HostAndPort.h"

namespace fastoredis
{
    class IConnectionSettingsBase
    {
    public:

        virtual std::string fullAdress() const = 0;

        virtual hostAndPort host() const = 0;
        virtual void setHost(const hostAndPort &host) = 0;

        std::string connectionName() const;
        void setConnectionName(const std::string &name);

        virtual connectionTypes connectionType() const;
        static IConnectionSettingsBase *fromStdString(const std::string &val);
        std::string toString() const;

        virtual IConnectionSettingsBase* clone () const = 0;
        virtual ~IConnectionSettingsBase();
    protected:
        virtual std::string toStringImpl() const = 0;
        virtual void initFromStringImpl(const std::string &val) = 0;

        IConnectionSettingsBase(const std::string &connectionName);
        std::string connectionName_;
    };

    typedef boost::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        typedef IConnectionSettingsBase base_class;
        struct config {
            hostAndPort host;
            std::string hostsocket;
            int dbnum;
            std::string auth;
        };
        RedisConnectionSettings(const std::string &connectionName, const config &info = config());

        virtual std::string fullAdress() const;

        virtual hostAndPort host() const;
        virtual void setHost(const hostAndPort &host);

        virtual connectionTypes connectionType() const;

        config info() const;
        void setInfo(const config& info);

        virtual IConnectionSettingsBase* clone () const;
    private:
        virtual std::string toStringImpl() const;
        virtual void initFromStringImpl(const std::string &val);
        config info_;
    };

    typedef boost::shared_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;
}
