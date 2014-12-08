#include "core/python_engine.h"

#include <QThread>

#include <QApplication>

#include "common/qt/convert_string.h"

#include "core/settings_manager.h"
#include "core/events/events.h"

#ifdef PYTHON_ENABLED
#include <iostream>
#include "PythonQtStdIn.h"
#include "PythonQtStdOut.h"
#include "PythonQtObjectPtr.h"
#endif

#define PYTHON_SHELL_VERSION "Unknown"

namespace
{
    enum InitFlags {
      RedirectStdOut = 1,   //!<< sets if the std out/err is redirected to pythonStdOut() and pythonStdErr() signals
      IgnoreSiteModule = 2, //!<< sets if Python should ignore the site module
      ExternalHelp = 4,     //!<< sets if help() calls on PythonQt modules are forwarded to the pythonHelpRequest() signal
      PythonAlreadyInitialized = 8 //!<< sets that PythonQt should not can PyInitialize, since it is already done
    };
}

namespace fastoredis
{
    namespace
    {
        void stdOutRedirectCB(const QString& str)
        {
            Q_EMIT PythonEngine::instance().pythonStdOut(str);
        }

        void stdErrRedirectCB(const QString& str)
        {
            Q_EMIT PythonEngine::instance().pythonStdErr(str);
        }
    }

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

        PyObject* result = PyRun_String(ptr, Py_file_input, globalDictionary, localDictionary);
        Py_DECREF(result);
        Py_DECREF(main);
        Py_DECREF(globalDictionary);
        Py_DECREF(localDictionary);
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
        int flags = IgnoreSiteModule | RedirectStdOut;

        // add our own python object types for redirection of stdout
        if (PyType_Ready(&PythonQtStdOutRedirectType) < 0) {
          std::cerr << "could not initialize PythonQtStdOutRedirectType" << ", in " << __FILE__ << ":" << __LINE__ << std::endl;
        }
        Py_INCREF(&PythonQtStdOutRedirectType);

        // add our own python object types for redirection of stdin
        if (PyType_Ready(&PythonQtStdInRedirectType) < 0) {
          std::cerr << "could not initialize PythonQtStdInRedirectType" << ", in " << __FILE__ << ":" << __LINE__ << std::endl;
        }
        Py_INCREF(&PythonQtStdInRedirectType);
        initPythonQtModule(flags & RedirectStdOut);
#endif
    }

    void PythonEngine::initPythonQtModule(bool redirectStdOut)
    {
      Py_SetProgramName(PROJECT_NAME);  /* optional but recommended */
      Py_Initialize();

      PythonQtObjectPtr sys;
      sys.setNewRef(PyImport_ImportModule("sys"));

      if (redirectStdOut) {
        PythonQtObjectPtr out;
        PythonQtObjectPtr err;
        // create a redirection object for stdout and stderr
        out = PythonQtStdOutRedirectType.tp_new(&PythonQtStdOutRedirectType,NULL, NULL);
        ((PythonQtStdOutRedirect*)out.object())->_cb = stdOutRedirectCB;
        err = PythonQtStdOutRedirectType.tp_new(&PythonQtStdOutRedirectType,NULL, NULL);
        ((PythonQtStdOutRedirect*)err.object())->_cb = stdErrRedirectCB;
        // replace the built in file objects with our own objects
        PyModule_AddObject(sys, "stdout", out);
        PyModule_AddObject(sys, "stderr", err);
      }

      // add PythonQt to the list of builtin module names
      PyObject *old_module_names = PyObject_GetAttrString(sys.object(),"builtin_module_names");
      if (old_module_names && PyTuple_Check(old_module_names)) {
        Py_ssize_t old_size = PyTuple_Size(old_module_names);
        PyObject *module_names = PyTuple_New(old_size + 1);
        for (Py_ssize_t i = 0; i < old_size; i++) {
          PyTuple_SetItem(module_names, i, PyTuple_GetItem(old_module_names, i));
        }
        PyTuple_SetItem(module_names, old_size, PyString_FromString(PROJECT_NAME));
        PyModule_AddObject(sys.object(), "builtin_module_names", module_names);
      }
      Py_XDECREF(old_module_names);

    #ifdef PY3K
      PyDict_SetItem(PyObject_GetAttrString(sys.object(), "modules"), PyUnicode_FromString(name.constData()), _p->_pythonQtModule.object());
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
//        Py_Finalize();
#endif
    }
}
