#include "core/lua_engine.h"

#include <QThread>
#include <QApplication>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "common/qt/convert_string.h"
#include "common/file_system.h"
#include "common/sprintf.h"
#include "common/utils.h"

#include "core/events/events.h"
#include "core/logger.h"

namespace fastoredis
{
    LuaWorker::LuaWorker()
        : stop_(false), lua_(NULL)
    {
        lua_ = luaL_newstate();
        DCHECK(lua_);
        if(lua_){
            luaL_openlibs(lua_);
        }
    }

    LuaWorker::~LuaWorker()
    {
        if(lua_){
            lua_close(lua_);
        }
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
        emit executeProgress(0);
        if(lua_){
            emit executeProgress(50);
            int res = luaL_dostring(lua_, script.c_str());
            if(res != LUA_OK){
                const char * s = lua_tostring(lua_, -1);
                if (s == NULL){
                    Q_EMIT luaStdErr("unrecognized Lua error");
                }
                else{
                    Q_EMIT luaStdErr(common::convertFromString<QString>(s));
                }
            }
        }
        emit executeProgress(100);
    }

    void LuaWorker::executeScriptImpl(const std::string& path, const std::vector<std::string>& args)
    {
        emit executeProgress(0);
        if(args.size() == 1 && args[0] == "install"){
            QString pathApp = QCoreApplication::instance()->applicationDirPath();
            std::string pathAppS = common::convertToString(pathApp);
            common::file_system::Path p2(pathAppS);
            p2.append("lua");
            p2.append(common::file_system::get_file_name(path));
            std::string pathAppStable = p2.path();
            bool res = common::file_system::copy_file(path, pathAppStable);
            if (!res){
                char buff[1024];
                common::SNPrintf(buff, sizeof(buff), "Install module in path %s to %s failed!", path, pathAppStable);
                Q_EMIT luaStdErr(common::convertFromString<QString>(buff));
            }
            else{
                char buff[1024];
                common::SNPrintf(buff, sizeof(buff), "Install module in path %s to %s successful!", path, pathAppStable);
                Q_EMIT luaStdOut(common::convertFromString<QString>(buff));
            }
            emit executeProgress(100);
            return;
        }

        if(lua_){
            emit executeProgress(50);
            int res = luaL_loadfile(lua_, path.c_str());
            if(res != LUA_OK){
                const char * s = lua_tostring(lua_, -1);
                if (s == NULL){
                    Q_EMIT luaStdErr("unrecognized Lua error");
                }
                else{
                    Q_EMIT luaStdOut(common::convertFromString<QString>(s));
                }
                goto done;
            }

            res = lua_pcall(lua_, 0, 0, 0);
            if(res != LUA_OK){
                const char * s = lua_tostring(lua_, -1);
                if (s == NULL){
                    Q_EMIT luaStdErr("unrecognized Lua error");
                }
                else{
                    Q_EMIT luaStdOut(common::convertFromString<QString>(s));
                }
            }
        }

    done:
        emit executeProgress(100);
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

    LuaEngine::~LuaEngine()
    {
    }

    const char* LuaEngine::version()
    {
        return LUA_VERSION;
    }

    namespace
    {
        std::string luaIndexToString(lua_State *L, int i);

        std::string luaTableToString(lua_State *L)
        {
            if(!L){
                return std::string();
            }

            lua_pushnil(L);
            std::string tret;
            while(lua_next(L, -2)) {
                tret += luaIndexToString(L, -2);
                tret += luaIndexToString(L, -1);
                lua_pop(L, 1);
            }
            return tret;
        }

        std::string luaIndexToString(lua_State *L, int i)
        {
            if(!L){
                return std::string();
            }

            int ltype = lua_type(L, i);
            if(ltype == LUA_TBOOLEAN){
                return lua_toboolean(L, i) ? "true" : "false";
            }
            else if(ltype == LUA_TSTRING){
                size_t len = 0;
                const char* sptr = lua_tolstring(L, i, &len);
                return common::utils::null2empty(sptr, len);
            }
            else if(ltype == LUA_TNUMBER){
                return common::convertToString(lua_tonumber(L, i));
            }
            else if(ltype == LUA_TTABLE){
                return luaTableToString(L);
            }
            else{
                NOTREACHED();
            }

            return std::string();
        }
    }

    LuaWorker* LuaEngine::createWorker()
    {
        LuaWorker* worker = new LuaWorker;
        QThread* thread = new LuaQThread(worker);
        worker->moveToThread(thread);

        VERIFY(QObject::connect(thread, &QThread::started, worker, &LuaWorker::init));
        thread->start();

        return worker;
    }


    bool LuaEngine::hasModule(const std::string& name)
    {
        return false;
    }
}
