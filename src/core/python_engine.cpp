#include "core/python_engine.h"

#include <QThread>

#include <QApplication>

#include "common/qt/convert_string.h"

#include "core/settings_manager.h"
#include "core/events/events.h"

#ifdef PYTHON_ENABLED
#include <Python.h>
#endif

#define PYTHON_SHELL_VERSION "Unknown"

namespace fastoredis
{
    bool isPythonEnabled()
    {
    #ifdef PYTHON_ENABLED
        return true;
    #else
        return false;
    #endif
    }

    PythonWorker::PythonWorker(int id)
        : id_(id)
    {
    }

    PythonWorker::~PythonWorker()
    {

    }

    void PythonWorker::stop()
    {

    }

    int PythonWorker::id() const
    {
        return id_;
    }

    void PythonWorker::init()
    {

    }

    void PythonWorker::customEvent(QEvent *event)
    {
        using namespace Events;
        QEvent::Type type = event->type();
        if (type == static_cast<QEvent::Type>(ExecuteRequestEvent::EventType)){
            ExecuteRequestEvent *ev = static_cast<ExecuteRequestEvent*>(event);
            ExecuteRequestEvent::value_type val = ev->value();
            execute(val.text_);
        }
    }

    void PythonWorker::execute(const std::string& script)
    {
        const char* ptr = script.c_str();
        int len = strlen(ptr);
    }

    const char* PythonEngine::version()
    {
#ifdef PYTHON_ENABLED
        return PYTHON_SHELL_VERSION;
#else
        return PYTHON_SHELL_VERSION;
#endif
    }

    PythonEngine::PythonEngine()
    {
#ifdef PYTHON_ENABLED
        Py_SetProgramName(PROJECT_NAME);  /* optional but recommended */
        Py_Initialize();
#endif
    }

    std::string PythonEngine::execPath() const
    {
        return SettingsManager::instance().pythonExecPath();
    }

    void PythonEngine::execute(PythonWorker* worker, const QString& script)
    {
        EventsInfo::ExecuteInfoRequest req(common::convertToString(script));
        QEvent *ev = new Events::ExecuteRequestEvent(this, req);
        qApp->postEvent(worker, ev);
    }

    PythonWorker* PythonEngine::createWorker()
    {
        PythonWorker* worker = new PythonWorker(generateId());
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        VERIFY(connect(thread, SIGNAL(started()), worker, SLOT(init())));
        thread->start();

        return worker;
    }

    void PythonEngine::stopThreads()
    {

    }

    int PythonEngine::generateId()
    {
        static int count = 0;
        return ++count;
    }

    PythonEngine::~PythonEngine()
    {
#ifdef PYTHON_ENABLED
        Py_Finalize();
#endif
    }
}
