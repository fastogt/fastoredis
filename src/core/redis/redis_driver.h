#pragma once

/**/

#include <boost/scoped_ptr.hpp>

#include "core/idriver.h"

namespace fastoredis
{
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

        static const QStringList &allCommands();
        static const QStringList &typesKeywords();
        static const QStringList &commandsKeywords();

    protected:
        virtual void customEvent(QEvent *event);
        virtual void timerEvent(QTimerEvent * event);
        virtual void initImpl();

        virtual void connectEvent(Events::ConnectRequestEvent *ev);
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev);
        virtual void executeEvent(Events::ExecuteRequestEvent *ev);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev);
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev);

    private:
        struct pimpl;
        boost::scoped_ptr<pimpl> impl_;
    };
}
