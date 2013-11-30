#include "core/IDriver.h"

#include <QThread>
#include <QApplication>
#include "common/macros.h"

#include "core/ConnectionSettings.h"
#include "core/events/Events.hpp"

namespace
{
    void postEvent(QObject* sender, QEvent *ev, int priority = Qt::NormalEventPriority)
    {
        qApp->postEvent(sender, ev, priority);
    }
}

namespace fastoredis
{
    IDriver::IDriver(const IConnectionSettingsBasePtr &settings)
        :_settings(settings)
    {
        _thread = new QThread(this);
        moveToThread(_thread);
        VERIFY(connect( _thread, SIGNAL(started()), this, SLOT(init()) ));
        _thread->start();
    }

    void IDriver::customEvent(QEvent *event)
    {
        using namespace Events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ConnectRequestEvent::EventType)){
            ConnectRequestEvent *ev = static_cast<ConnectRequestEvent*>(event);
            ConnectRequestEvent::value_type v = ev->value();

            ConnectResponceEvent::value_type res(v);
            connectImpl(res);
            postEvent(ev->sender(), new ConnectResponceEvent(this,res));
        }
        else if (type == static_cast<QEvent::Type>(DisconnectRequestEvent::EventType)){
            DisconnectRequestEvent *ev = static_cast<DisconnectRequestEvent*>(event);
            DisconnectRequestEvent::value_type v = ev->value();

            DisconnectResponceEvent::value_type res(v);
            disconnectImpl(res);
            postEvent(ev->sender(), new DisconnectResponceEvent(this,res));
        }
        else if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            ExecuteRequestEvent::value_type v = ev->value();

            ExecuteResponceEvent::value_type res(v);
            executeImpl(res);
            postEvent(ev->sender(), new ExecuteResponceEvent(this,res));
        }
        return base_class::customEvent(event);
    }

    void IDriver::init()
    {
        initImpl();
    }

    connectionTypes IDriver::connectionType() const
    {
        return _settings->connectionType();
    }

    const IConnectionSettingsBasePtr &IDriver::settings() const
    {
        return _settings;
    }

    IDriver::~IDriver()
    {

    }
}
