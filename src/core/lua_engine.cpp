#include "core/lua_engine.h"

#include <QThread>
#include <QApplication>
#include <QCoreApplication>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "common/qt/convert_string.h"

#include "core/events/events.h"
#include "core/logger.h"

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
        lua_State *L = luaL_newstate();

        // load the libs
        luaL_openlibs(L);

        //run a Lua scrip here
        luaL_dostring(L, script.c_str());

        lua_close(L);
    }

    void LuaWorker::executeScriptImpl(const std::string& path, const std::vector<std::string>& args)
    {
        lua_State *L = luaL_newstate();

        // load the libs
        luaL_openlibs(L);

        //run a Lua scrip here
        luaL_dofile(L, path.c_str());

        lua_close(L);
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

    const char* LuaEngine::version()
    {
        return LUA_VERSION;
    }

    namespace
    {
        class LuaQThread
                : public QThread
        {
            LuaWorker* worker_;
        public:
            LuaQThread(LuaWorker* worker)
                : worker_(worker)
            {

            }

            LuaWorker* worker() const
            {
                return worker_;
            }
        };

        LuaWorker* workerByThreadID()
        {
            QThread* thr = QThread::currentThread();
            if(thr){
                LuaQThread* lthr = dynamic_cast<LuaQThread*>(thr);
                if(lthr){
                    return lthr->worker();
                }
            }

            return NULL;
        }

        void printStdOut(const std::string& data)
        {
            LuaWorker* worker = workerByThreadID();
            if(worker){
                Q_EMIT worker->luaStdOut(common::convertFromString<QString>(data));
            }
            else{
                LOG_MSG(data, common::logging::L_WARNING, true);
            }
        }

        void printStdErr(const std::string& data)
        {
            LuaWorker* worker = workerByThreadID();
            if(worker){
                Q_EMIT worker->luaStdErr(common::convertFromString<QString>(data));
            }
            else{
                LOG_MSG(data, common::logging::L_WARNING, true);
            }
        }

        int redirect_fprintf(FILE* stream, const char* msg, ...)
        {
            char buffer[1024];

            va_list args;
            va_start(args, msg);
            vsprintf(buffer, msg, args);
            va_end(args);

            if (stream == stdout){
                printStdOut(buffer);
            }
            else{
                printStdErr(buffer);
            }

            return strlen(buffer);
        }

        // fputs hook for Lua output redirection
        int redirect_fputs(const char* msg, FILE* stream)
        {
            return redirect_fprintf(stream, msg);
        }

        // fwrite hook for Lua output redirection
        int redirect_fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
        {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, ptr, size * count);

            if (stream == stdout){
                printStdOut(buffer);
            }
            else{
                printStdErr(buffer);
            }

            return count;
        }

        // printf hook for Lua output redirection
        int redirect_printf(const char* msg, ...)
        {
            char buffer[1024];

            va_list args;
            va_start(args, msg);
            vsprintf(buffer, msg, args);
            va_end(args);

            printStdOut(buffer);

            return strlen(buffer);
        }
    }

    LuaEngine::LuaEngine()
    {
        lua_fprintf = redirect_fprintf;
        lua_fputs   = redirect_fputs;
        lua_fwrite  = redirect_fwrite;
        lua_printf  = redirect_printf;
    }

    bool LuaEngine::hasModule(const std::string& name)
    {
        return false;
    }

    LuaWorker* LuaEngine::createWorker()
    {
        LuaWorker* worker = new LuaWorker;
        QThread* thread = new LuaQThread(worker);
        worker->moveToThread(thread);

        VERIFY(QObject::connect(thread, SIGNAL(started()), worker, SLOT(init())));
        thread->start();

        return worker;
    }

    LuaEngine::~LuaEngine()
    {
    }
}
