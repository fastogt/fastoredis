#pragma once

#include <QObject>

#include "common/net/net.h"

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
            : public QObject, private IFastoObjectObserver
    {
        Q_OBJECT
    public:
        static void reply(QObject* reciver, QEvent* ev);
        virtual ~IDriver();

        connectionTypes connectionType() const;
        IConnectionSettingsBaseSPtr settings() const;

        //sync
        void start();
        void stop();

        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;
        virtual common::net::hostAndPort address() const = 0;
        virtual std::string version() const = 0;
        virtual std::string outputDelemitr() const = 0;
        DataBaseInfoSPtr currentDatabaseInfo() const;

    Q_SIGNALS:
        void addedChild(FastoObject* child);
        void itemUpdated(FastoObject* item, const QString& val);
        void serverInfoSnapShoot(ServerInfoSnapShoot shot);

    private Q_SLOTS:
        void init();
        void clear();

    protected:        
        virtual void timerEvent(QTimerEvent* event);

        IDriver(const IConnectionSettingsBaseSPtr &settings);
        void notifyProgress(QObject *reciver, int value);

        virtual void customEvent(QEvent *event);

    protected:
        virtual void initImpl() = 0;
        virtual void clearImpl() = 0;
        virtual common::ErrorValueSPtr currentLoggingInfo(ServerInfo** info) = 0;

        virtual void handleConnectEvent(events::ConnectRequestEvent* ev) = 0;
        virtual void handleDisconnectEvent(events::DisconnectRequestEvent* ev) = 0;
        virtual void handleExecuteEvent(events::ExecuteRequestEvent* ev) = 0;
        virtual void handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoRequestEvent* ev) = 0;
        virtual void handleLoadServerInfoEvent(events::ServerInfoRequestEvent* ev) = 0;
        virtual void handleLoadServerPropertyEvent(events::ServerPropertyInfoRequestEvent* ev) = 0;
        virtual void handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoRequestEvent* ev) = 0;
        virtual void handleDbValueChangeEvent(events::ChangeDbValueRequestEvent* ev) = 0;
        virtual void handleShutdownEvent(events::ShutDownRequestEvent* ev) = 0;
        virtual void handleBackupEvent(events::BackupRequestEvent* ev) = 0;
        virtual void handleExportEvent(events::ExportRequestEvent* ev) = 0;

// ============== database =============//
        virtual void handleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent* ev) = 0;
        virtual void handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseRequestEvent* ev) = 0;
// ============== database =============//

// ============== command =============//
        virtual void handleCommandRequestEvent(events::CommandRequestEvent* ev) = 0;
// ============== command =============//

        const IConnectionSettingsBaseSPtr settings_;

        class RootLocker
        {
        public:
            RootLocker(IDriver* parent, QObject* reciver, const std::string& text);
            ~RootLocker();

            FastoObjectIPtr root_;

        private:
            FastoObjectIPtr createRoot(QObject* reciver, const std::string& text);

            IDriver* parent_;
            QObject* reciver_;
        };

        RootLocker make_locker(QObject* reciver, const std::string& text)
        {
            return RootLocker(this, reciver, text);
        }

    private:
        void handleLoadServerInfoHistoryEvent(events::ServerInfoHistoryRequestEvent *ev);

        virtual void addedChildren(FastoObject *child);
        virtual void updated(FastoObject* item, common::Value* val);

        virtual ServerInfoSPtr makeServerInfoFromString(const std::string& val) = 0;
        virtual void handleProcessCommandLineArgs(events::ProcessConfigArgsRequestEvent* ev) = 0;

    protected:
        DataBaseInfoSPtr currentDatabaseInfo_;

    private:
        QThread* thread_;
        int timer_info_id_;
        common::file_system::File* log_file_;
    };
}
