#include "core/lua_engine.h"

#include <QThread>
#include <QApplication>
#include <QCoreApplication>

#include "common/file_system.h"
#include "common/qt/convert_string.h"

#include "core/events/events.h"

namespace fastoredis
{
    LuaWorker::LuaWorker()
        : stop_(false)
    {

    }

    LuaWorker::~LuaWorker()
    {

    }

    void LuaWorker::stop()
    {
        stop_ = true;
    }

    void LuaWorker::init()
    {

    }

    void LuaWorker::customEvent(QEvent* event)
    {
        using namespace events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            ExecuteRequestEvent::value_type val = ev->value();
            if(!stop_){
                executeImpl(val.text_, val.args_);
            }            
        }
        else if(type == static_cast<QEvent::Type>(ExecuteScriptRequestEvent::EventType)){
            ExecuteScriptRequestEvent *ev = static_cast<ExecuteScriptRequestEvent*>(event);
            ExecuteScriptRequestEvent::value_type val = ev->value();
            if(!stop_){
                executeScriptImpl(val.path_, val.args_);
            }
        }
        stop_ = false;

        QObject::customEvent(event);
    }

    void LuaWorker::executeImpl(const std::string& script, const std::vector<std::string>& args)
    {

    }

    void LuaWorker::executeScriptImpl(const std::string& path, const std::vector<std::string>& args)
    {

    }

    void LuaWorker::execute(const QString& script, const QStringList &args)
    {
        std::vector<std::string> sargs;
        for(QStringList::const_iterator it = args.begin(); it < args.end(); ++it){
            QString val = *it;
            sargs.push_back(common::convertToString(val));
        }

        EventsInfo::ExecuteInfoRequest req(common::convertToString(script), sargs);
        QEvent *ev = new events::ExecuteRequestEvent(this, req);
        qApp->postEvent(this, ev);
    }

    void LuaWorker::executeScript(const QString& path, const QStringList& args)
    {
        std::vector<std::string> sargs;
        for(QStringList::const_iterator it = args.begin(); it < args.end(); ++it){
            QString val = *it;
            sargs.push_back(common::convertToString(val));
        }

        EventsInfo::ExecuteScriptInfoRequest req(common::convertToString(path), sargs);
        QEvent *ev = new events::ExecuteScriptRequestEvent(this, req);
        qApp->postEvent(this, ev);
    }

    bool LuaWorker::handleError()
    {
        bool flag = false;
        return flag;
    }

    const char* LuaEngine::version()
    {
        return "";
    }

    LuaEngine::LuaEngine()
    {

    }

    bool LuaEngine::hasModule(const std::string& name)
    {
        return false;
    }

    LuaWorker* LuaEngine::createWorker()
    {
        LuaWorker* worker = new LuaWorker;
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        VERIFY(QObject::connect(thread, SIGNAL(started()), worker, SLOT(init())));
        thread->start();

        return worker;
    }

    LuaEngine::~LuaEngine()
    {
    }
}
