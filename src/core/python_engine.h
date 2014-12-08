#pragma once

/**/

#include <QObject>
#include <QString>

#include "global/global.h"

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

        ~PythonWorker();

    Q_SIGNALS:
        void textOut(const QString& data);

    private Q_SLOTS:
        void init();

    protected:
        virtual void customEvent(QEvent *event);

    private:
        void execute(const std::string& script);

        PythonWorker(int id);        

        const int id_;
    };

    class PythonEngine
            : public QObject
    {
        Q_OBJECT
    public:
        static const char* version();
        PythonEngine();
        ~PythonEngine();

        std::string execPath() const;

        void execute(PythonWorker* worker, const QString& script);
        PythonWorker* createWorker() WARN_UNUSED_RESULT;
        void stopThreads();

    private:
        int generateId();
    };
}
