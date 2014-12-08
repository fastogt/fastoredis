#pragma once

/**/

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

        int id() const;
        void stop();

    private Q_SLOTS:
        void init();

    protected:
        virtual void customEvent(QEvent *event);

    private:
        void execute(const std::string& script);

        PythonWorker(int id);
        ~PythonWorker();

        const int id_;
    };

    class PythonEngine
            : public QObject, public common::patterns::lazy_singleton<PythonEngine>
    {
        friend class common::patterns::lazy_singleton<PythonEngine>;

        Q_OBJECT
    public:
        static const char* version();

        std::string execPath() const;

        void execute(PythonWorker* worker, const QString& script);
        PythonWorker* createWorker() WARN_UNUSED_RESULT;
        void stopThreads();

    private:
        int generateId();

        PythonEngine();
        ~PythonEngine();
    };
}
