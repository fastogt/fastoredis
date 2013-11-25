#include "core/RedisClient.h"

#include <QThread>
#include "common/macros.h"

namespace fastoredis
{
    RedisClient::RedisClient(const RedisConnectionSettings &settings)
        :settings_(settings)
    {
        _thread = new QThread(this);
        moveToThread(_thread);
        VERIFY(connect( _thread, SIGNAL(started()), this, SLOT(init()) ));
        _thread->start();
    }

    void RedisClient::customEvent(QEvent *event)
    {
        return base_class::customEvent(event);
    }

    void RedisClient::init()
    {

    }
}
