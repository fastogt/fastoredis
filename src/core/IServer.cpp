#include "core/IServer.h"

#include <QApplication>

#include "core/ConnectionSettings.h"
#include "core/ServersManager.h"
#include "core/events/Events.hpp"
#include "common/qt/convert_string.h"

namespace
{
    using namespace fastoredis;

    struct connectFunct
    {
        explicit connectFunct(bool isConnect) : isConnect_(isConnect) {}
        bool operator()(const QObject *sender, const char *signal, const QObject *receiver, const char *member, Qt::ConnectionType type) const
        {
            if(isConnect_){
                return QObject::connect(sender, signal, receiver, member, type);
            }
            else{
                return QObject::disconnect(sender, signal, receiver, member);
            }
        }

        const bool isConnect_;
    };

    void syncServersFunct(IServer *src, IServer *dsc, const connectFunct &func)
    {
        VERIFY(func(src, SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), dsc, SIGNAL(startedConnect(const EventsInfo::ConnectInfoRequest &)), Qt::UniqueConnection));
        VERIFY(func(src, SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), dsc, SIGNAL(finishedConnect(const EventsInfo::ConnectInfoResponce &)), Qt::UniqueConnection));
        VERIFY(func(src, SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), dsc, SIGNAL(startedDisconnect(const EventsInfo::DisonnectInfoRequest &)), Qt::UniqueConnection));
        VERIFY(func(src, SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), dsc, SIGNAL(finishedDisconnect(const EventsInfo::DisConnectInfoResponce &)), Qt::UniqueConnection));
        VERIFY(func(src, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), dsc, SIGNAL(startedExecute(const EventsInfo::ExecuteInfoRequest &)), Qt::UniqueConnection));
        VERIFY(func(src, SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)), dsc, SIGNAL(finishedExecute(const EventsInfo::ExecuteInfoResponce &)), Qt::UniqueConnection));
   }
}

namespace fastoredis
{
    IServer::IServer(const IDriverPtr &drv, bool isMaster)
        : _drv(drv), _isMaster(isMaster)
    {
    }

    void IServer::unSyncServers(IServer *src, IServer *dsc)
    {
        static const connectFunct f(false);
        syncServersFunct(src, dsc, f);
        syncServersFunct(dsc, src, f);
    }

    void IServer::syncServers(IServer *src, IServer *dsc)
    {
        static const connectFunct f(true);
        syncServersFunct(src, dsc, f);
        syncServersFunct(dsc, src, f);
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

    void IServer::loadDatabaseContent(const DataBaseInfo &inf)
    {
        EventsInfo::LoadDatabasesContentRequest req(inf);
        emit startedLoadDataBaseContent(req);
        QEvent *ev = new Events::LoadDatabaseContentRequestEvent(this, req);
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

    void IServer::serverInfo()
    {
        EventsInfo::ServerInfoRequest req;
        emit startedLoadServerInfo(req);
        QEvent *ev = new Events::ServerInfoRequestEvent(this, req);
        notify(ev);
    }

    void IServer::serverProperty()
    {
        EventsInfo::ServerPropertyInfoRequest req;
        emit startedLoadServerProperty(req);
        QEvent *ev = new Events::ServerPropertyInfoRequestEvent(this, req);
        notify(ev);
    }

    void IServer::changeProperty(const PropertyType &newValue)
    {
        EventsInfo::ChangeServerPropertyInfoRequest req;
        req.newItem_ = newValue;
        emit startedChangeServerProperty(req);
        QEvent *ev = new Events::ChangeServerPropertyInfoRequestEvent(this, req);
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

    bool IServer::isMaster() const
    {
        return _isMaster;
    }

    void IServer::setIsMaster(bool isMaster)
    {
        _isMaster = isMaster;
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
            connectEvent(ev);
        }
        else if(type == static_cast<QEvent::Type>(DisconnectResponceEvent::EventType))
        {
            DisconnectResponceEvent *ev = static_cast<DisconnectResponceEvent*>(event);
            disconnectEvent(ev);
        }
        else if(type == static_cast<QEvent::Type>(ExecuteResponceEvent::EventType))
        {
            ExecuteResponceEvent *ev = static_cast<ExecuteResponceEvent*>(event);
            executeEvent(ev);
        }
        else if(type == static_cast<QEvent::Type>(LoadDatabasesInfoResponceEvent::EventType))
        {
            LoadDatabasesInfoResponceEvent *ev = static_cast<LoadDatabasesInfoResponceEvent*>(event);
            loadDatabaseInfosEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(LoadDatabaseContentResponceEvent::EventType)){
            LoadDatabaseContentResponceEvent *ev = static_cast<LoadDatabaseContentResponceEvent*>(event);
            loadDatabaseContentEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerInfoResponceEvent::EventType)){
            ServerInfoResponceEvent *ev = static_cast<ServerInfoResponceEvent*>(event);
            loadServerInfoEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerPropertyInfoResponceEvent::EventType)){
            ServerPropertyInfoResponceEvent *ev = static_cast<ServerPropertyInfoResponceEvent*>(event);
            loadServerPropertyEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ChangeServerPropertyInfoResponceEvent::EventType)){
            ChangeServerPropertyInfoResponceEvent *ev = static_cast<ChangeServerPropertyInfoResponceEvent*>(event);
            serverPropertyChangeEvent(ev);
        }
        else if(type == static_cast<QEvent::Type>(ProgressResponceEvent::EventType))
        {
            ProgressResponceEvent *ev = static_cast<ProgressResponceEvent*>(event);
            ProgressResponceEvent::value_type v = ev->value();
            emit progressChanged(v);
        }
        return QObject::customEvent(event);
    }
}
