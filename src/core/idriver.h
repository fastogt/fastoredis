#pragma once

#include <boost/shared_ptr.hpp>

#include <QObject>

#include "core/connection_settings.h"
#include "core/events/events.hpp"

class QThread;

namespace fastoredis
{
    class IDriver
            : public QObject
    {
        Q_OBJECT
    public:
        static void reply(QObject *reciver, QEvent *ev, bool silent);

        connectionTypes connectionType() const;
        const IConnectionSettingsBasePtr &settings() const;
        virtual ~IDriver();

        //sync
        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;
        virtual std::string address() const = 0;

    private Q_SLOTS:
        void init();

    protected:
        IDriver(const IConnectionSettingsBasePtr &settings);
        void notifyProgress(QObject *reciver, int value, bool silent);

        virtual void customEvent(QEvent *event);
        virtual void initImpl() = 0;

        virtual void connectEvent(Events::ConnectRequestEvent *ev, bool silent = false) = 0;
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev, bool silent = false) = 0;
        virtual void executeEvent(Events::ExecuteRequestEvent *ev, bool silent = false) = 0;
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev, bool silent = false) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev, bool silent = false) = 0;
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev, bool silent = false) = 0;
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev, bool silent = false) = 0;
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev, bool silent = false) = 0;

    private:
        QThread *thread_;

    protected:
        const IConnectionSettingsBasePtr settings_;
    };

    typedef boost::shared_ptr<IDriver> IDriverPtr;
}
