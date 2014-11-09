#pragma once

/**/

#include <QObject>

#include "core/connection_settings.h"
#include "core/events/events.h"

class QThread;

namespace common
{
    namespace file_system
    {
        class File;
    }
}

namespace fastoredis
{
    class IDriver
            : public QObject
    {
        Q_OBJECT
    public:
        static void reply(QObject *reciver, QEvent *ev);
        virtual ~IDriver();

        connectionTypes connectionType() const;
        const IConnectionSettingsBasePtr &settings() const;

        //sync
        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;
        virtual std::string address() const = 0;

    private Q_SLOTS:
        void init();

    protected:        
        virtual void timerEvent(QTimerEvent* event);

        IDriver(const IConnectionSettingsBasePtr &settings);
        void notifyProgress(QObject *reciver, int value);

        virtual void customEvent(QEvent *event);

    protected:
        virtual void initImpl() = 0;
        virtual common::ErrorValueSPtr currentLoggingInfo(FastoObjectPtr& outInfo) = 0;

        virtual void connectEvent(Events::ConnectRequestEvent *ev) = 0;
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev) = 0;
        virtual void executeEvent(Events::ExecuteRequestEvent *ev) = 0;
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev) = 0;
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev) = 0;
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev) = 0;
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev) = 0;
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev) = 0;

        const IConnectionSettingsBasePtr settings_;

    private:
        void loadServerInfoHistoryEvent(Events::ServerInfoHistoryRequestEvent *ev);

        virtual ServerInfoSPtr makeServerInfoFromString(const std::string& val) = 0;

        QThread *thread_;
        int timer_info_id_;
        common::file_system::File* logFile_;
    };

    typedef boost::shared_ptr<IDriver> IDriverPtr;
}
