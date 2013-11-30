#include "core/IServer.h"

#include <QApplication>

#include "core/ConnectionSettings.h"
#include "core/ServersManager.h"
#include "core/events/Events.hpp"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    IServer::IServer(const IDriverPtr &drv)
        : _drv(drv)
    {

    }

    IServer::IServer(const IServerPtr &srv)
        : _drv(srv->driver())
    {
        VERIFY(QObject::connect(srv.get(),SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),this,SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(srv.get(),SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),this,SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(srv.get(),SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),this,SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(srv.get(),SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),this,SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(srv.get(),SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),this,SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(srv.get(),SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),this,SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),Qt::DirectConnection));
    }

    IDriverPtr IServer::driver() const
    {
        return _drv;
    }

    QString IServer::name() const
    {
        return common::utils_qt::toQString(_drv->settings()->connectionName());
    }

    connectionTypes IServer::connectionType() const
    {
        return _drv->connectionType();
    }

    void IServer::connect()
    {
        EventsInfo::ConnectInfoRequest req;
        emit startedConnect(req);
        QEvent *ev = new Events::ConnectRequestEvent(this,req);
        notify(ev);
    }

    void IServer::disconnect()
    {
        EventsInfo::DisonnectInfoRequest req;
        emit startedDisconnect(req);
        QEvent *ev = new Events::DisconnectRequestEvent(this,req);
        notify(ev);
    }

    void IServer::execute(const QString &script)
    {
        EventsInfo::ExecuteInfoRequest req(common::utils_qt::toStdString(script));
        emit startedExecute(req);
        QEvent *ev = new Events::ExecuteRequestEvent(this,req);
        notify(ev);
    }

    IServer::~IServer()
    {

    }

    void IServer::notify(QEvent *ev)
    {
        qApp->postEvent(_drv.get(),ev);
    }

    void IServer::customEvent(QEvent *event)
    {
        using namespace Events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ConnectResponceEvent::EventType)){
            ConnectResponceEvent *ev = static_cast<ConnectResponceEvent*>(event);
            ConnectResponceEvent::value_type v = ev->value();
            emit finishedConnect(v);
        }
        else if(type == static_cast<QEvent::Type>(DisconnectResponceEvent::EventType))
        {
            DisconnectResponceEvent *ev = static_cast<DisconnectResponceEvent*>(event);
            DisconnectResponceEvent::value_type v = ev->value();
            emit finishedDisconnect(v);
        }
        else if(type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType))
        {
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            ExecuteRequestEvent::value_type v = ev->value();
            emit finishedExecute(v);
        }
        return base_class::customEvent(event);
    }
}
