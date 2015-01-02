#pragma once

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

        RedisDriver(const IConnectionSettingsBaseSPtr& settings);
        virtual ~RedisDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        common::net::hostAndPort address() const;
        std::string version() const;
        virtual std::string outputDelemitr() const;

        static const char* versionApi();

    private:
        virtual void customEvent(QEvent *event);
        virtual void initImpl();
        virtual common::ErrorValueSPtr currentLoggingInfo(FastoObject *out);

        virtual void handleConnectEvent(Events::ConnectRequestEvent* ev);
        virtual void handleDisconnectEvent(Events::DisconnectRequestEvent* ev);
        virtual void handleExecuteEvent(Events::ExecuteRequestEvent* ev);
        virtual void handleLoadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent* ev);
        virtual void handleLoadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent* ev);
        virtual void handleLoadServerInfoEvent(Events::ServerInfoRequestEvent* ev);
        virtual void handleLoadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent* ev);
        virtual void handleServerPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent* ev);
        virtual void handleProcessCommandLineArgs(Events::ProcessConfigArgsRequestEvent* ev);
        virtual void handleShutdownEvent(Events::ShutDownRequestEvent* ev);
        virtual void handleBackupEvent(Events::BackupRequestEvent* ev);
        virtual void handleExportEvent(Events::ExportRequestEvent* ev);

        ServerInfoSPtr makeServerInfoFromString(const std::string& val);

        struct pimpl;
        pimpl* const impl_;

        common::ErrorValueSPtr interacteveMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr latencyMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr slaveMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr getRDBMode(Events::ProcessConfigArgsRequestEvent* ev);
        //void pipeMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr findBigKeysMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr statMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr scanMode(Events::ProcessConfigArgsRequestEvent* ev);
    };
}
