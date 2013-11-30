#pragma once

#include "core/IDriver.h"

namespace fastoredis
{
    class RedisDriver
            : public IDriver
    {
        Q_OBJECT
    public:
        typedef IDriver base_class;
        RedisDriver(const IConnectionSettingsBasePtr &settings);
        virtual ~RedisDriver();

    protected:
        void customEvent(QEvent *event);
        virtual void initImpl();
        virtual void connectImpl(const EventsInfo::ConnectInfoResponce &res);
        virtual void executeImpl(const EventsInfo::ExecuteInfoResponce &res);
        virtual void disconnectImpl(const EventsInfo::DisConnectInfoResponce &res);
    };
}
