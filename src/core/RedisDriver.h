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
        virtual void connectImpl(EventsInfo::ConnectInfoResponce &res);
        virtual void executeImpl(EventsInfo::ExecuteInfoResponce &res);
        virtual void disconnectImpl(EventsInfo::DisConnectInfoResponce &res);
    };
}
