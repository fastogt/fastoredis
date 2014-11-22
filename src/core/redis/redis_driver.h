#pragma once

/**/

#include "core/idriver.h"

namespace fastoredis
{
    extern const std::vector<QString> redisTypesKeywords;
    extern const std::vector<QString> redisCommandsKeywords;

    class RedisDriver
            : public IDriver
    {
        Q_OBJECT
    public:        

        RedisDriver(const IConnectionSettingsBasePtr &settings);
        virtual ~RedisDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        std::string address() const;
        std::string version() const;
        static const char* versionApi();

    private:
        virtual void customEvent(QEvent *event);
        virtual void initImpl();
        virtual common::ErrorValueSPtr currentLoggingInfo(FastoObjectPtr& outInfo);

        virtual void connectEvent(Events::ConnectRequestEvent *ev);
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev);
        virtual void executeEvent(Events::ExecuteRequestEvent *ev);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev);
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev);

        ServerInfoSPtr makeServerInfoFromString(const std::string &val);

        struct pimpl;
        boost::scoped_ptr<pimpl> impl_;
    };
}
