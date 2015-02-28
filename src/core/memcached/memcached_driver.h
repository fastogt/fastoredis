#pragma once

#include "core/idriver.h"

namespace fastoredis
{
    const std::vector<QString> memcachedCommandsKeywords = {"quit",
                                                            "verbosity", "version", "stats", "flush_all",
                                                            "delete", "decr", "incr", "prepend", "append", "replace",
                                                            "add", "set", "get"};
    //get [key]
    //set [key] [value] [time-to-live-seconds] [flags]
    //add

    common::ErrorValueSPtr testConnection(MemcachedConnectionSettings* settings);

    class MemcachedDriver
            : public IDriver
    {
        Q_OBJECT
    public:        

        MemcachedDriver(const IConnectionSettingsBaseSPtr& settings);
        virtual ~MemcachedDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        common::net::hostAndPort address() const;
        std::string version() const;
        virtual std::string outputDelemitr() const;
        std::string commandByType(CommandKey::cmdtype type, const std::string& name, common::Value::Type vtype);

        static const char* versionApi();

    private:
        virtual void customEvent(QEvent *event);
        virtual void initImpl();
        virtual void clearImpl();
        virtual common::ErrorValueSPtr currentLoggingInfo(ServerInfo** info);

        virtual void handleConnectEvent(events::ConnectRequestEvent* ev);
        virtual void handleDisconnectEvent(events::DisconnectRequestEvent* ev);
        virtual void handleExecuteEvent(events::ExecuteRequestEvent* ev);
        virtual void handleLoadDatabaseInfosEvent(events::LoadDatabasesInfoRequestEvent* ev);
        virtual void handleLoadServerInfoEvent(events::ServerInfoRequestEvent* ev);
        virtual void handleLoadServerPropertyEvent(events::ServerPropertyInfoRequestEvent* ev);
        virtual void handleServerPropertyChangeEvent(events::ChangeServerPropertyInfoRequestEvent* ev);
        virtual void handleDbValueChangeEvent(events::ChangeDbValueRequestEvent* ev);
        virtual void handleProcessCommandLineArgs(events::ProcessConfigArgsRequestEvent* ev);
        virtual void handleShutdownEvent(events::ShutDownRequestEvent* ev);
        virtual void handleBackupEvent(events::BackupRequestEvent* ev);
        virtual void handleExportEvent(events::ExportRequestEvent* ev);

// ============== database =============//
        virtual void handleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent* ev);
        virtual void handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseRequestEvent* ev);
// ============== database =============//
// ============== command =============//
        virtual void handleCommandRequestEvent(events::CommandRequestEvent* ev);
// ============== command =============//
        ServerInfoSPtr makeServerInfoFromString(const std::string& val);

        struct pimpl;
        pimpl* const impl_;
    };
}
