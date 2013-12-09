#pragma once

#include <boost/shared_ptr.hpp>

#include "common/boost_extension.hpp"
#include "core/ConnectionTypes.h"
#include "core/RedisConfig.h"

namespace fastoredis
{
    class IConnectionSettingsBase
    {
    public:
        virtual std::string commandLine() const = 0;
        virtual void setCommandLine(const std::string &line) = 0;

        virtual std::string fullAddress() const = 0;

        virtual std::string host() const = 0;
        virtual void setHost(const std::string &host) = 0;
        virtual int port() const = 0;
        virtual void setPort(int port) = 0;


        std::string connectionName() const;
        void setConnectionName(const std::string &name);

        virtual connectionTypes connectionType() const;
        static IConnectionSettingsBase *fromStdString(const std::string &val);
        std::string toString() const;

        virtual IConnectionSettingsBase* clone () const = 0;
        virtual ~IConnectionSettingsBase();
    protected:
        virtual std::string toCommandLine() const = 0;
        virtual void initFromCommandLine(const std::string &val) = 0;

        IConnectionSettingsBase(const std::string &connectionName);
        std::string connectionName_;
    };

    typedef boost::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBasePtr;

    class RedisConnectionSettings
            : public IConnectionSettingsBase
    {
    public:
        typedef IConnectionSettingsBase base_class;
        RedisConnectionSettings(const std::string &connectionName, const redisConfig &info = redisConfig());

        virtual std::string commandLine() const;
        virtual void setCommandLine(const std::string &line);

        virtual std::string fullAddress() const;

        virtual std::string host() const;
        virtual void setHost(const std::string &host);
        virtual int port() const;
        virtual void setPort(int port);

        virtual connectionTypes connectionType() const;

        redisConfig info() const;
        void setInfo(const redisConfig& info);

        virtual IConnectionSettingsBase* clone () const;
    private:
        virtual std::string toCommandLine() const;
        virtual void initFromCommandLine(const std::string &val);
        redisConfig info_;
    };

    typedef boost::shared_ptr<RedisConnectionSettings> RedisConnectionSettingsPtr;
}
