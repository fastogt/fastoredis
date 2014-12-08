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
#define OUT_FILE_NAME "out.tmp"

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
        if(!ptr){
            return;
        }

        PyObject* main = PyImport_AddModule("__main__");
        if(!main){
            Py_DECREF(main);
            return;
        }

        PyObject* globalDictionary = PyModule_GetDict(main);
        if(!globalDictionary){
            Py_DECREF(main);
            return;
        }
        PyObject* localDictionary = PyDict_New();
        if(!localDictionary){
            Py_DECREF(globalDictionary);
            Py_DECREF(main);
            return;
        }

        fflush(stdout);
        FILE* pythonOutput = freopen(OUT_FILE_NAME, "w", stdout);
        if(!pythonOutput){
            return;
        }

        PyObject* result = PyRun_String(ptr, Py_file_input, globalDictionary, localDictionary);
        Py_DECREF(result);
        Py_DECREF(main);
        Py_DECREF(globalDictionary);
        Py_DECREF(localDictionary);

        fclose(pythonOutput);

        pythonOutput = fopen(OUT_FILE_NAME, "r");
        if(!pythonOutput){
            return;
        }

        int readed = 0;
        const unsigned int size = 1024;
        char buff[size];
        QString out;
        while ((readed = fread(buff, sizeof(char), size, pythonOutput))){
            if(readed == -1){
                break;
            }

            std::string data(buff, readed);
            out += common::convertFromString<QString>(data);
        }

        emit textOut(out);
        fclose(pythonOutput);
        remove(OUT_FILE_NAME);
        fflush(stdout);
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
