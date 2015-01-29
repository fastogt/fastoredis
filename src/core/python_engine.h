#pragma once

#include <QObject>
#include <QString>

#include "global/global.h"
#include "common/patterns/singleton_pattern.h"

namespace fastoredis
{
    bool isPythonEnabled();

    class PythonWorker
            : public QObject
    {
        Q_OBJECT
    public:
        friend class PythonEngine;

        void execute(const QString& script, const QStringList& args);
        void executeScript(const QString& path, const QStringList& args);
        void stop();

        ~PythonWorker();

    Q_SIGNALS:
        //! emitted when python outputs something to stdout (and redirection is turned on)
        void pythonStdOut(const QString& str);
        //! emitted when python outputs something to stderr (and redirection is turned on)
        void pythonStdErr(const QString& str);
        //! emitted when both custom SystemExit exception handler is enabled and a SystemExit
        //! exception is raised.
        //! \sa setSystemExitExceptionHandlerEnabled(bool)
        void systemExitExceptionRaised(int exitCode);
        void executeProgress(int val);

    private Q_SLOTS:
        void init();

    protected:
        virtual void customEvent(QEvent *event);

    private:
        void executeImpl(const std::string& script, const std::vector<std::string>& args);
        void executeScriptImpl(const std::string& path, const std::vector<std::string>& args);

        bool handleError();
        PythonWorker();
        volatile bool stop_;
    };

    class PythonEngine
            : public common::patterns::LazySingleton<PythonEngine>
    {
        friend class common::patterns::LazySingleton<PythonEngine>;

    public:
        static const char* version();
        PythonWorker* createWorker() WARN_UNUSED_RESULT;
        bool hasModule(const std::string& name);

    private:
        PythonEngine();
        ~PythonEngine();
    };
}
