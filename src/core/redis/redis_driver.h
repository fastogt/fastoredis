#pragma once

/**/

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

        RedisDriver(const IConnectionSettingsBasePtr &settings);
        virtual ~RedisDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        std::string address() const;
        std::string version() const;
        virtual std::string outputDelemitr() const;

        static const char* versionApi();

    private:
        virtual void customEvent(QEvent *event);
        virtual void initImpl();
        virtual common::ErrorValueSPtr currentLoggingInfo(FastoObjectPtr& outInfo);

        virtual void connectEvent(Events::ConnectRequestEvent *ev);
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev);
        virtual void executeEvent(Events::ExecuteRequestEvent *ev);
        virtual void loadDatabaseInfosEvent(Events::LoadDatabasesInfoRequestEvent *ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentRequestEvent *ev);
        virtual void loadServerInfoEvent(Events::ServerInfoRequestEvent *ev);
        virtual void loadServerPropertyEvent(Events::ServerPropertyInfoRequestEvent *ev);
        virtual void serverPropertyChangeEvent(Events::ChangeServerPropertyInfoRequestEvent *ev);
        virtual void processCommandLineArgs(Events::ProcessConfigArgsRequestEvent* ev);

        ServerInfoSPtr makeServerInfoFromString(const std::string &val);

        struct pimpl;
        pimpl* const impl_;

        common::ErrorValueSPtr interacteveMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr latencyMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr slaveMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr getRDBMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        //void pipeMode(Events::ProcessConfigArgsRequestEvent* ev);
        common::ErrorValueSPtr findBigKeysMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr statMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
        common::ErrorValueSPtr scanMode(Events::ProcessConfigArgsRequestEvent* ev) WARN_UNUSED_RESULT;
    };
}
