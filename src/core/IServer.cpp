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
        syncServers(srv.get(), this);
    }

    void IServer::syncServers(IServer *src, IServer *dsc)
    {
        VERIFY(QObject::connect(src,SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),dsc,SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(src,SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),dsc,SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(src,SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),dsc,SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(src,SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),dsc,SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(src,SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),dsc,SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(src,SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),dsc,SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),Qt::DirectConnection));

        VERIFY(QObject::connect(dsc,SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),src,SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(dsc,SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),src,SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(dsc,SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),src,SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(dsc,SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),src,SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)),Qt::DirectConnection));
        VERIFY(QObject::connect(dsc,SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),src,SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)),Qt::DirectConnection));
        VERIFY(QObject::connect(dsc,SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),src,SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)),Qt::DirectConnection));
    }

    IDriverPtr IServer::driver() const
    {
        return _drv;
    }

    QString IServer::name() const
    {
        return common::utils_qt::toQString(_drv->settings()->connectionName());
    }

    QString IServer::address() const
    {
        return common::utils_qt::toQString(_drv->address());
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

    void IServer::loadDatabases()
    {
        EventsInfo::LoadDatabasesInfoRequest req;
        emit startedLoadDatabases(req);
        QEvent *ev = new Events::LoadDatabasesInfoRequestEvent(this,req);
        notify(ev);
    }

    void IServer::disconnect()
    {
        EventsInfo::DisonnectInfoRequest req;
        emit startedDisconnect(req);
        QEvent *ev = new Events::DisconnectRequestEvent(this, req);
        notify(ev);
    }

    void IServer::execute(const QString &script)
    {
        EventsInfo::ExecuteInfoRequest req(common::utils_qt::toStdString(script));
        emit startedExecute(req);
        QEvent *ev = new Events::ExecuteRequestEvent(this, req);
        notify(ev);
    }

    void IServer::stopCurrentEvent()
    {
        _drv->interrupt();
    }

    bool IServer::isConnected() const
    {
        return _drv->isConnected();
    }

    IServer::~IServer()
    {

    }

    void IServer::notify(QEvent *ev)
    {
        emit progressChanged(0);
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
        else if(type == static_cast<QEvent::Type>(ExecuteResponceEvent::EventType))
        {
            ExecuteResponceEvent *ev = static_cast<ExecuteResponceEvent*>(event);
            ExecuteResponceEvent::value_type v = ev->value();
            emit finishedExecute(v);
        }
        else if(type == static_cast<QEvent::Type>(LoadDatabasesInfoResponceEvent::EventType))
        {
            LoadDatabasesInfoResponceEvent *ev = static_cast<LoadDatabasesInfoResponceEvent*>(event);
            LoadDatabasesInfoResponceEvent::value_type v = ev->value();
            emit finishedLoadDatabases(v);
        }
        else if(type == static_cast<QEvent::Type>(ProgressResponceEvent::EventType))
        {
            ProgressResponceEvent *ev = static_cast<ProgressResponceEvent*>(event);
            ProgressResponceEvent::value_type v = ev->value();
            emit progressChanged(v);
        }
        return base_class::customEvent(event);
    }
}
