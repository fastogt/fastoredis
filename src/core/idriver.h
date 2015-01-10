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
        const IConnectionSettingsBaseSPtr& settings() const;

        //sync
        void start();
        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;
        virtual common::net::hostAndPort address() const = 0;
        virtual std::string version() const = 0;
        virtual std::string outputDelemitr() const = 0;

    Q_SIGNALS:
        void addedChild(FastoObject* child);
        void itemUpdated(FastoObject* item, const QString& val);
        void serverInfoSnapShoot(ServerInfoSnapShoot shot);

    private Q_SLOTS:
        void init();

    protected:        
        virtual void timerEvent(QTimerEvent* event);

        IDriver(const IConnectionSettingsBaseSPtr &settings);
        void notifyProgress(QObject *reciver, int value);

        virtual void customEvent(QEvent *event);

    protected:
        virtual void initImpl() = 0;
        virtual common::ErrorValueSPtr currentLoggingInfo(FastoObject* out) = 0;

        virtual void handleConnectEvent(Events::ConnectRequestEvent* ev) = 0;
        virtual void handleDisconnectEvent(Events::DisconnectRequestEvent* ev) = 0;
        virtual void handleExecuteEvent(Events::ExecuteRequestEvent* ev) = 0;
        virtual void handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent* ev) = 0;
        virtual void handleLoadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent* ev) = 0;
        virtual void handleLoadServerInfoEvent(Events::ServerInfoRequestEvent* ev) = 0;
        virtual void handleLoadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent* ev) = 0;
        virtual void handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent* ev) = 0;
        virtual void handleShutdownEvent(Events::ShutDownRequestEvent* ev) = 0;
        virtual void handleBackupEvent(Events::BackupRequestEvent* ev) = 0;
        virtual void handleExportEvent(Events::ExportRequestEvent* ev) = 0;

        const IConnectionSettingsBaseSPtr settings_;

        class RootLocker
        {
        public:
            RootLocker(IDriver* parent, QObject* reciver, const std::string& text);
            ~RootLocker();

            FastoObjectIPtr root_;

        private:
            IDriver* parent_;
            QObject *reciver_;
        };

        RootLocker make_locker(QObject* reciver, const std::string& text)
        {
            return RootLocker(this, reciver, text);
        }

        void clear();

    private:
        FastoObjectIPtr createRoot(QObject* reciver, const std::string& text);
        void compleateRoot(QObject* reciver, FastoObjectIPtr root);

        void handleLoadServerInfoHistoryEvent(Events::ServerInfoHistoryRequestEvent *ev);

        virtual void addedChildren(FastoObject *child);
        virtual void updated(FastoObject* item, common::Value* val);

        virtual ServerInfoSPtr makeServerInfoFromString(const std::string& val) = 0;
        virtual void handleProcessCommandLineArgs(Events::ProcessConfigArgsRequestEvent* ev) = 0;

        QThread* thread_;
        int timer_info_id_;
        common::file_system::File* log_file_;
    };

    typedef shared_ptr_t<IDriver> IDriverSPtr;
}
