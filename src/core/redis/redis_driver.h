#pragma once

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

        virtual void connectEvent(Events::ConnectRequestEvent *ev, bool silent = false);
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev, bool silent = false);
        virtual void executeEvent(Events::ExecuteRequestEvent *ev, bool silent = false);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev, bool silent = false);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev, bool silent = false);
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev, bool silent = false);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev, bool silent = false);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev, bool silent = false);

    private:
        struct pimpl;
        boost::scoped_ptr<pimpl> impl_;
    };
}
