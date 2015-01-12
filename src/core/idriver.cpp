#include "core/idriver.h"

#ifdef OS_POSIX
#include <signal.h>
#endif

#include <QThread>
#include <QApplication>

#include "common/file_system.h"
#include "common/time.h"

namespace
{
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
#else
    struct SigIgnInit
    {
        SigIgnInit(){
            signal(SIGPIPE, SIG_IGN);
        }
    } sig_init;
#endif

    const char magicNumber = 0x1E;
    std::string createStamp(long long time)
    {
        return magicNumber + common::convertToString(time) + '\n';
    }

    bool getStamp(const common::buffer_type& stamp, long long& timeOut)
    {
        if(stamp.empty()){
            return false;
        }

        if(stamp[0] != magicNumber){
            return false;
        }

        common::buffer_type cstamp = stamp;

        if(cstamp[cstamp.size()-1] == '\n'){
            cstamp.resize(cstamp.size()-1);
        }

        timeOut = common::convertFromString<long long>((const char*)(cstamp.c_str() + 1));

        return timeOut != 0;
    }
}

namespace fastoredis
{
    IDriver::IDriver(const IConnectionSettingsBaseSPtr &settings)
        : settings_(settings), timer_info_id_(0), log_file_(NULL)
    {
        thread_ = new QThread(this);
        moveToThread(thread_);
        VERIFY(connect( thread_, SIGNAL(started()), this, SLOT(init()) ));        
    }

    IDriver::~IDriver()
    {
        delete log_file_;
        log_file_ = NULL;
    }

    void IDriver::start()
    {
        thread_->start();
    }

    void IDriver::clear()
    {
        killTimer(timer_info_id_);
        timer_info_id_ = 0;
        thread_->quit();
        thread_->wait();
    }

    void IDriver::customEvent(QEvent *event)
    {
        using namespace Events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ConnectRequestEvent::EventType)){            
            ConnectRequestEvent *ev = static_cast<ConnectRequestEvent*>(event);
            handleConnectEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ShutDownRequestEvent::EventType)){
            ShutDownRequestEvent *ev = static_cast<ShutDownRequestEvent*>(event);
            handleShutdownEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ProcessConfigArgsRequestEvent::EventType)){
            ProcessConfigArgsRequestEvent *ev = static_cast<ProcessConfigArgsRequestEvent*>(event);
            handleProcessCommandLineArgs(ev);
        }
        else if (type == static_cast<QEvent::Type>(DisconnectRequestEvent::EventType)){
            DisconnectRequestEvent *ev = static_cast<DisconnectRequestEvent*>(event);
            handleDisconnectEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            handleExecuteEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(LoadDatabasesInfoRequestEvent::EventType)){
            LoadDatabasesInfoRequestEvent *ev = static_cast<LoadDatabasesInfoRequestEvent*>(event);
            handleLoadDatabaseInfosEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(LoadDatabaseContentRequestEvent::EventType)){
            LoadDatabaseContentRequestEvent *ev = static_cast<LoadDatabaseContentRequestEvent*>(event);
            handleLoadDatabaseContentEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerInfoRequestEvent::EventType)){
            ServerInfoRequestEvent *ev = static_cast<ServerInfoRequestEvent*>(event);
            handleLoadServerInfoEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerInfoHistoryRequestEvent::EventType)){
            ServerInfoHistoryRequestEvent *ev = static_cast<ServerInfoHistoryRequestEvent*>(event);
            handleLoadServerInfoHistoryEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ServerPropertyInfoRequestEvent::EventType)){
            ServerPropertyInfoRequestEvent *ev = static_cast<ServerPropertyInfoRequestEvent*>(event);
            handleLoadServerPropertyEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ChangeServerPropertyInfoRequestEvent::EventType)){
            ChangeServerPropertyInfoRequestEvent *ev = static_cast<ChangeServerPropertyInfoRequestEvent*>(event);
            handleServerPropertyChangeEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(BackupRequestEvent::EventType)){
            BackupRequestEvent *ev = static_cast<BackupRequestEvent*>(event);
            handleBackupEvent(ev);
        }
        else if (type == static_cast<QEvent::Type>(ExportRequestEvent::EventType)){
            ExportRequestEvent *ev = static_cast<ExportRequestEvent*>(event);
            handleExportEvent(ev);
        }
        return QObject::customEvent(event);
    }

    IDriver::RootLocker::RootLocker(IDriver* parent, QObject *reciver, const std::string &text)
        : parent_(parent), reciver_(reciver)
    {
        DCHECK(parent_);
        root_ = parent_->createRoot(reciver, text);
    }

    IDriver::RootLocker::~RootLocker()
    {
        parent_->compleateRoot(reciver_, root_);
    }

    FastoObjectIPtr IDriver::createRoot(QObject *reciver, const std::string& text)
    {
        FastoObjectIPtr root = FastoObject::createRoot(text, this);
        Events::CommandRootCreatedEvent::value_type res(root);
        reply(reciver, new Events::CommandRootCreatedEvent(this, res));
        return root;
    }

    void IDriver::compleateRoot(QObject *reciver, FastoObjectIPtr root)
    {
        Events::CommandRootCompleatedEvent::value_type res(root);
        reply(reciver, new Events::CommandRootCompleatedEvent(this, res));
    }

    void IDriver::addedChildren(FastoObject* child)
    {
        DCHECK(child);
        if(!child){
            return;
        }

        emit addedChild(child);
    }

    void IDriver::updated(FastoObject* item, common::Value* val)
    {
        const QString value = common::convertFromString<QString>(val->toString());
        emit itemUpdated(item, value);
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
        int interval = settings_->loggingMsTimeInterval();
        timer_info_id_ = startTimer(interval);
        DCHECK(timer_info_id_);
        initImpl();
    }

    void IDriver::timerEvent(QTimerEvent* event)
    {
        if(timer_info_id_ == event->timerId() && isConnected() && settings_->loggingEnabled()){
            if(!log_file_){
                std::string path = settings_->loggingPath();
                std::string dir = common::file_system::get_dir_path(path);
                bool created = common::file_system::create_directory(dir, true);
                if(common::file_system::is_directory(dir) == SUCCESS){
                    common::file_system::Path p(path);
                    log_file_ = new common::file_system::File(p);
                }
            }

            if(log_file_ && !log_file_->isOpened()){
                bool opened = log_file_->open("ab+");
            }

            if(log_file_ && log_file_->isOpened()){
                long long time = common::time::current_mstime();
                FastoObjectIPtr toFile = FastoObject::createRoot(createStamp(time));
                FastoObject* ptr = toFile.get();
                common::ErrorValueSPtr er = currentLoggingInfo(ptr);
                if(er && er->isError()){
                    QObject::timerEvent(event);
                    return;
                }

                std::string data = common::convertToString(ptr);

                FastoObject::child_container_type ch = toFile->childrens();
                if(ch.size()){
                    std::string info = common::convertToString(ch[0]);
                    ServerInfoSnapShoot shot(time, makeServerInfoFromString(info));
                    emit serverInfoSnapShoot(shot);
                }

                log_file_->write(data);
                log_file_->flush();
            }
        }
        QObject::timerEvent(event);
    }

    connectionTypes IDriver::connectionType() const
    {
        return settings_->connectionType();
    }

    const IConnectionSettingsBaseSPtr &IDriver::settings() const
    {
        return settings_;
    }

    void IDriver::handleLoadServerInfoHistoryEvent(Events::ServerInfoHistoryRequestEvent *ev)
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
                if(!res || readFile.isEof()){
                    if(curStamp){
                        tmpInfos.push_back(ServerInfoSnapShoot(curStamp, makeServerInfoFromString(common::convertToString(dataInfo))));
                    }
                    break;
                }

                long long tmpStamp = 0;
                bool isSt = getStamp(data, tmpStamp);
                if(isSt){
                    if(curStamp){
                        tmpInfos.push_back(ServerInfoSnapShoot(curStamp, makeServerInfoFromString(common::convertToString(dataInfo))));
                    }
                    curStamp = tmpStamp;
                    dataInfo.clear();
                }
                else{
                    dataInfo += data;
                }
            }
            res.setInfos(tmpInfos);
        }
        else{
           common::ErrorValueSPtr er(new common::ErrorValue("Logging file not found", common::ErrorValue::E_ERROR));
           res.setErrorInfo(er);
        }

        reply(sender, new Events::ServerInfoHistoryResponceEvent(this, res));
    }
}
