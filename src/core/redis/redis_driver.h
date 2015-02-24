#pragma once

#include "core/idriver.h"

namespace fastoredis
{
    extern const std::vector<QString> redisTypesKeywords;
    extern const std::vector<QString> redisCommandsKeywords;

    common::ErrorValueSPtr testConnection(RedisConnectionSettings* settings);

    class RedisDriver
            : public IDriver
    {
        Q_OBJECT
    public:
        RedisDriver(const IConnectionSettingsBaseSPtr& settings);
        virtual ~RedisDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        common::net::hostAndPort address() const;
        std::string version() const;
        virtual std::string outputDelemitr() const;
        virtual std::string commandByType(CommandKey::cmdtype type);

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
        virtual void handleProcessCommandLineArgs(events::ProcessConfigArgsRequestEvent* ev);
        virtual void handleShutdownEvent(events::ShutDownRequestEvent* ev);
        virtual void handleBackupEvent(events::BackupRequestEvent* ev);
        virtual void handleExportEvent(events::ExportRequestEvent* ev);

// ============== database =============//
        virtual void handleDbValueChangeEvent(events::ChangeDbValueRequestEvent* ev);
        virtual void handleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent* ev);
        virtual void handleSetDefaultDatabaseEvent(events::SetDefaultDatabaseRequestEvent* ev);
// ============== database =============//
// ============== command =============//
        virtual void handleCommandRequestEvent(events::CommandRequestEvent* ev);
// ============== command =============//
        ServerInfoSPtr makeServerInfoFromString(const std::string& val);

        struct pimpl;
        pimpl* const impl_;

        common::ErrorValueSPtr interacteveMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr latencyMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr slaveMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr getRDBMode(events::ProcessConfigArgsRequestEvent* ev);
        //void pipeMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr findBigKeysMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr statMode(events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr scanMode(events::ProcessConfigArgsRequestEvent* ev);
    };
}
