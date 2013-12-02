#include "core/IDriver.h"

#include <QThread>
#include <QApplication>
#include "common/macros.h"

#include "core/ConnectionSettings.h"
#include "core/events/Events.hpp"

namespace fastoredis
{
    namespace detail
    {
        Progresser::Progresser(QObject *sen, QObject *rec) : _sender(sen), _reciver(rec)
        {
            qApp->postEvent(_reciver, new Events::ProgressResponceEvent(_sender, Events::ProgressResponceEvent::value_type(25)));
        }

        Progresser::~Progresser()
        {
            qApp->postEvent(_reciver, new Events::ProgressResponceEvent(_sender, Events::ProgressResponceEvent::value_type(75)));
        }
    }

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
            detail::Progresser(this, ev->sender());
            ConnectResponceEvent::value_type res(ev->value());
            connectImpl(res);
            reply(ev->sender(), new ConnectResponceEvent(this,res));
        }
        else if (type == static_cast<QEvent::Type>(DisconnectRequestEvent::EventType)){
            DisconnectRequestEvent *ev = static_cast<DisconnectRequestEvent*>(event);
            detail::Progresser(this, ev->sender());
            DisconnectResponceEvent::value_type res(ev->value());
            disconnectImpl(res);
            reply(ev->sender(), new DisconnectResponceEvent(this,res));
        }
        else if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            detail::Progresser(this, ev->sender());
            ExecuteResponceEvent::value_type res(ev->value());
            executeImpl(res);
            reply(ev->sender(), new ExecuteResponceEvent(this,res));
        }
        return base_class::customEvent(event);
    }

    void IDriver::reply(QObject *reciver, QEvent *ev)
    {
        qApp->postEvent(reciver, ev);
    }

    void IDriver::interuptEvent(const EventsInfo::InteruptInfoRequest &req, EventsInfo::InteruptInfoResponce &res)
    {

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
