#pragma once

#include <boost/shared_ptr.hpp>
#include <QObject>

class QThread;

#include "core/ConnectionSettings.h"
#include "core/events/Events.hpp"

namespace fastoredis
{
    class IDriver
            : public QObject
    {
        Q_OBJECT
    public:
        static void reply(QObject *reciver, QEvent *ev);

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
        void notifyProgress(QObject *reciver, int value);

        virtual void customEvent(QEvent *event);
        virtual void initImpl() = 0;

        virtual void connectEvent(Events::ConnectRequestEvent *ev) = 0;
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev) = 0;
        virtual void executeEvent(Events::ExecuteRequestEvent *ev) = 0;
        virtual void loadDatabasesInfoEvent(Events::LoadDatabasesInfoRequestEvent *ev) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev) = 0;
        virtual void serverInfoEvent(Events::ServerInfoRequestEvent *ev) = 0;
        virtual void serverPropertyEvent(Events::ServerPropertyRequestEvent *ev) = 0;
        virtual void serverPropertyChangeEvent(Events::ServerPropertyChangeRequestEvent *ev) = 0;

    private:
        QThread *_thread;

    protected:
        const IConnectionSettingsBasePtr _settings;        
    };

    typedef boost::shared_ptr<IDriver> IDriverPtr;
}
