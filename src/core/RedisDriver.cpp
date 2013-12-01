#include "core/RedisDriver.h"

namespace fastoredis
{
    RedisDriver::RedisDriver(const IConnectionSettingsBasePtr &settings)
        :base_class(settings)
    {
    }

    void RedisDriver::customEvent(QEvent *event)
    {
        return base_class::customEvent(event);
    }

    void RedisDriver::initImpl()
    {

    }

    RedisDriver::~RedisDriver()
    {

    }

    void RedisDriver::connectImpl(EventsInfo::ConnectInfoResponce &res)
    {

    }

    void RedisDriver::executeImpl(EventsInfo::ExecuteInfoResponce &res)
    {

    }

    void RedisDriver::disconnectImpl(EventsInfo::DisConnectInfoResponce &res)
    {

    }
}
