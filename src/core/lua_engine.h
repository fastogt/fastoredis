#pragma once

#include <QObject>
#include <QString>

#include "global/global.h"
#include "common/patterns/singleton_pattern.h"

typedef struct lua_State lua_State;

namespace fastoredis
{
    class LuaWorker
            : public QObject
    {
        Q_OBJECT
    public:
        friend class LuaEngine;

        void execute(const QString& script, const QStringList& args);
        void executeScript(const QString& path, const QStringList& args);
        void stop();

        ~LuaWorker();

    Q_SIGNALS:
        void luaStdOut(const QString& str);
        void luaStdErr(const QString& str);
        void executeProgress(int val);

    private Q_SLOTS:
        void init();

    protected:
        virtual void customEvent(QEvent *event);

    private:
        void executeImpl(const std::string& script, const std::vector<std::string>& args);
        void executeScriptImpl(const std::string& path, const std::vector<std::string>& args);

        LuaWorker();
        volatile bool stop_;
        lua_State* lua_;
    };

    class LuaEngine
            : public common::patterns::LazySingleton<LuaEngine>
    {
        friend class common::patterns::LazySingleton<LuaEngine>;

    public:
        static const char* version();
        LuaWorker* createWorker() WARN_UNUSED_RESULT;
        bool hasModule(const std::string& name);

    private:
        LuaEngine();
        ~LuaEngine();
    };
}
