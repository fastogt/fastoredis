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

namespace
{
    const common::char16 magicNumber = 0x1E;
    common::string16 createStamp()
    {
        long long time = common::time::current_mstime();
        return magicNumber + common::convertToString16(time);
    }

    bool getStamp(const common::buffer_type& stamp, long long& timeOut)
    {
        if(stamp.empty()){
            return false;
        }

        if(stamp[0] != magicNumber){
            return false;
        }

        timeOut = common::convertFromString<long long>((const char*)(stamp.c_str() + 1));

        return timeOut != 0;
    }
}

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
        else if (type == static_cast<QEvent::Type>(ServerInfoHistoryRequestEvent::EventType)){
            ServerInfoHistoryRequestEvent *ev = static_cast<ServerInfoHistoryRequestEvent*>(event);
            loadServerInfoHistoryEvent(ev);
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
                std::string path = settings_->loggingPath();
                std::string dir = common::file_system::get_dir_path(path);
                common::file_system::create_directory(dir, true);
                if(common::file_system::is_directory(dir) == SUCCESS){
                    common::file_system::Path p(path);
                    logFile_ = new common::file_system::File(p);
                }
            }

            if(logFile_ && !logFile_->isOpened()){
                logFile_->open("ab+");
            }
            if(logFile_ && logFile_->isOpened()){
                FastoObjectPtr outInf;
                common::ErrorValueSPtr er = currentLoggingInfo(outInf);
                if(er && er->isError()){
                    QObject::timerEvent(event);
                    return;
                }

                FastoObject* par = FastoObject::createRoot(createStamp());
                FastoObjectPtr toFile = outInf->deepCopyChangeParent(par);
                common::string16 data = common::convertToString16(toFile.get());
                logFile_->write(data);
                logFile_->flush();
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

    void IDriver::loadServerInfoHistoryEvent(Events::ServerInfoHistoryRequestEvent *ev)
    {
        QObject *sender = ev->sender();
        Events::ServerInfoHistoryResponceEvent::value_type res(ev->value());        

        std::string path = settings_->loggingPath();
        common::file_system::Path p(path);

        common::file_system::File readFile(p);
        if(readFile.open("rb")){
            Events::ServerInfoHistoryResponceEvent::value_type::infos_container_type tmpInfos;

            long long curStamp = 0;
            common::buffer_type dataInfo;

            while(!readFile.isEof()){
                common::buffer_type data;
                bool res = readFile.readLine(data);
                if(!res){
                    if(curStamp){
                        tmpInfos[curStamp] = makeServerInfoFromString(common::convertToString(dataInfo));
                    }
                    break;
                }

                long long tmpStamp = 0;
                bool isSt = getStamp(data, tmpStamp);
                if(isSt){
                    if(curStamp){
                        tmpInfos[curStamp] = makeServerInfoFromString(common::convertToString(dataInfo));
                    }
                    curStamp = tmpStamp;
                    dataInfo.clear();
                }
                else{
                    dataInfo += data;
                }
            }
            res.infos_ = tmpInfos;
        }
        else{
           common::ErrorValueSPtr er(new common::ErrorValue(common::convertToString16("Logging file not found"), common::ErrorValue::E_ERROR));
           res.setErrorInfo(er);
        }

        reply(sender, new Events::ServerInfoHistoryResponceEvent(this, res));
    }
}
