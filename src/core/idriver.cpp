#include "core/idriver.h"

#include <QThread>
#include <QApplication>
#include <QDateTime>
#include "common/file_system.h"
#include "common/time.h"

#ifdef OS_WIN
#include <winsock2.h>
struct WinsockInit {
        WinsockInit() {
            WSADATA d;
            if ( WSAStartup(MAKEWORD(2,2), &d) != 0 ) {
                _exit(1);
            }
        }
        ~WinsockInit(){ WSACleanup(); }
    } winsock_init;
#endif

namespace fastoredis
{
    IDriver::IDriver(const IConnectionSettingsBasePtr &settings)
        : settings_(settings), timer_info_id_(0), logFile_(NULL)
    {
        thread_ = new QThread(this);
        moveToThread(thread_);
        VERIFY(connect( thread_, SIGNAL(started()), this, SLOT(init()) ));
        thread_->start();
    }

    void IDriver::customEvent(QEvent *event)
    {
        using namespace Events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ConnectRequestEvent::EventType)){
            ConnectRequestEvent *ev = static_cast<ConnectRequestEvent*>(event);            
            connectEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(DisconnectRequestEvent::EventType)){
            DisconnectRequestEvent *ev = static_cast<DisconnectRequestEvent*>(event);
            disconnectEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);           
            executeEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(LoadDatabasesInfoRequestEvent::EventType)){
            LoadDatabasesInfoRequestEvent *ev = static_cast<LoadDatabasesInfoRequestEvent*>(event);
            loadDatabaseInfosEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(LoadDatabaseContentRequestEvent::EventType)){
            LoadDatabaseContentRequestEvent *ev = static_cast<LoadDatabaseContentRequestEvent*>(event);
            loadDatabaseContentEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerInfoRequestEvent::EventType)){
            ServerInfoRequestEvent *ev = static_cast<ServerInfoRequestEvent*>(event);
            loadServerInfoEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerPropertyInfoRequestEvent::EventType)){
            ServerPropertyInfoRequestEvent *ev = static_cast<ServerPropertyInfoRequestEvent*>(event);
            loadServerPropertyEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ChangeServerPropertyInfoRequestEvent::EventType)){
            ChangeServerPropertyInfoRequestEvent *ev = static_cast<ChangeServerPropertyInfoRequestEvent*>(event);
            serverPropertyChangeEvent(ev);
        }
        return QObject::customEvent(event);
    }

    void IDriver::reply(QObject *reciver, QEvent *ev)
    {
        qApp->postEvent(reciver, ev);
    }

    void IDriver::notifyProgress(QObject *reciver, int value)
    {
        reply(reciver, new Events::ProgressResponceEvent(this, Events::ProgressResponceEvent::value_type(value)));
    }

    void IDriver::init()
    {
        timer_info_id_ = startTimer(60000);
        DCHECK(timer_info_id_);
        initImpl();
    }

    void IDriver::timerEvent(QTimerEvent* event)
    {
        if(timer_info_id_ == event->timerId() && isConnected() && settings_->loggingEnabled()){
            if(!logFile_){
                common::unicode_string path = settings_->loggingPath();
                common::unicode_string dir = common::file_system::get_dir_path(path);
                common::file_system::create_directory(dir, true);
                if(common::file_system::is_directory(dir) == common::SUCCESS){
                    common::file_system::Path p(path);
                    logFile_ = new common::file_system::File(p);
                }
            }

            if(logFile_ && !logFile_->isOpened()){
                logFile_->open("wb+");
            }
            if(logFile_ && logFile_->isOpened()){
                FastoObjectPtr outInf;
                common::ErrorValue er = currentLoggingInfo(outInf);
                if(!er.isError()){
                    long long time = common::time::current_mstime();
                    FastoObject* par = FastoObject::createRoot(common::convert2string(time));
                    FastoObjectPtr toFile = outInf->deepCopyChangeParent(par);
                    common::unicode_string data = common::convert2string(toFile.get());
                    logFile_->write(data);
                }
            }
        }
        QObject::timerEvent(event);
    }

    connectionTypes IDriver::connectionType() const
    {
        return settings_->connectionType();
    }

    const IConnectionSettingsBasePtr &IDriver::settings() const
    {
        return settings_;
    }

    IDriver::~IDriver()
    {
        killTimer(timer_info_id_);
        timer_info_id_ = 0;
        thread_->quit();
        if (!thread_->wait(2000)){
            thread_->terminate();
        }
        delete logFile_;
    }
}
