#include "core/python_engine.h"

#include <QThread>

#include <QApplication>

#include "common/qt/convert_string.h"

#include "core/settings_manager.h"
#include "core/events/events.h"

#ifdef PYTHON_ENABLED
#include <iostream>
#include "PythonQtStdOut.h"
#include "PythonQtObjectPtr.h"
namespace
{
    PythonQtObjectPtr getMainModule()
    {
        //both borrowed
        PythonQtObjectPtr dict = PyImport_GetModuleDict();
        return PyDict_GetItemString(dict, "__main__");
    }

    //! adapted from python source file "pythonrun.c", function "handle_system_exit"
    //! return the exitcode instead of calling "Py_Exit".
    //! it gives the application an opportunity to properly terminate.
    int custom_system_exit_exception_handler()
    {
      PyObject *exception, *value, *tb;
      int exitcode = 0;

    //  if (Py_InspectFlag)
    //    /* Don't exit if -i flag was given. This flag is set to 0
    //     * when entering interactive mode for inspecting. */
    //    return exitcode;

      PyErr_Fetch(&exception, &value, &tb);
    #ifndef PY3K
      if (Py_FlushLine()) {
        PyErr_Clear();
      }
    #else
      std::cout << std::endl;
    #endif
      fflush(stdout);
      if (value == NULL || value == Py_None)
        goto done;
      if (PyExceptionInstance_Check(value)) {
        /* The error code should be in the `code' attribute. */
        PyObject *code = PyObject_GetAttrString(value, "code");
        if (code) {
          Py_DECREF(value);
          value = code;
          if (value == Py_None)
            goto done;
        }
        /* If we failed to dig out the 'code' attribute,
           just let the else clause below print the error. */
      }
    #ifdef PY3K
      if (PyLong_Check(value))
        exitcode = (int)PyLong_AsLong(value);
    #else
      if (PyInt_Check(value))
        exitcode = (int)PyInt_AsLong(value);
    #endif
      else {
        PyObject *sys_stderr = PySys_GetObject(const_cast<char*>("stderr"));
        if (sys_stderr != NULL && sys_stderr != Py_None) {
          PyFile_WriteObject(value, sys_stderr, Py_PRINT_RAW);
        } else {
          PyObject_Print(value, stderr, Py_PRINT_RAW);
          fflush(stderr);
        }
        PySys_WriteStderr("\n");
        exitcode = 1;
      }
      done:
        /* Restore and clear the exception info, in order to properly decref
         * the exception, value, and traceback.      If we just exit instead,
         * these leak, which confuses PYTHONDUMPREFS output, and may prevent
         * some finalizers from running.
         */
        PyErr_Restore(exception, value, tb);
        PyErr_Clear();
        return exitcode;
        //Py_Exit(exitcode);
    }
}
#endif

#define PYTHON_SHELL_VERSION "Unknown"


namespace fastoredis
{
    namespace
    {
        void stdOutRedirectCB(const QString& str, void* data)
        {
            PythonWorker* worker = static_cast<PythonWorker*>(data);
            if(worker){
                Q_EMIT worker->pythonStdOut(str);
            }
        }

        void stdErrRedirectCB(const QString& str, void* data)
        {
            PythonWorker* worker = static_cast<PythonWorker*>(data);
            if(worker){
                Q_EMIT worker->pythonStdErr(str);
            }
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
#ifdef PYTHON_ENABLED
        // add our own python object types for redirection of stdout
        if (PyType_Ready(&PythonQtStdOutRedirectType) < 0) {
          std::cerr << "could not initialize PythonQtStdOutRedirectType" << ", in " << __FILE__ << ":" << __LINE__ << std::endl;
        }
        Py_INCREF(&PythonQtStdOutRedirectType);

        Py_SetProgramName(PROJECT_NAME);  /* optional but recommended */
        Py_Initialize();

        PythonQtObjectPtr sys;
        sys.setNewRef(PyImport_ImportModule("sys"));

          PythonQtObjectPtr out;
          PythonQtObjectPtr err;
          // create a redirection object for stdout and stderr
          out = PythonQtStdOutRedirectType.tp_new(&PythonQtStdOutRedirectType,NULL, NULL);
          ((PythonQtStdOutRedirect*)out.object())->_cb = stdOutRedirectCB;
          ((PythonQtStdOutRedirect*)out.object())->data = this;
          err = PythonQtStdOutRedirectType.tp_new(&PythonQtStdOutRedirectType,NULL, NULL);
          ((PythonQtStdOutRedirect*)err.object())->_cb = stdErrRedirectCB;
          ((PythonQtStdOutRedirect*)err.object())->data = this;
          // replace the built in file objects with our own objects
          PyModule_AddObject(sys, "stdout", out);
          PyModule_AddObject(sys, "stderr", err);

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
#endif
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
emit executeProgress(0);

        const char* ptr = script.c_str();
        if(!ptr){
            return;
        }

        PythonQtObjectPtr main = getMainModule();
emit executeProgress(25);
        PythonQtObjectPtr p;
        PyObject* dict = NULL;
        if (PyModule_Check(main)) {
            dict = PyModule_GetDict(main);
        }
        else if (PyDict_Check(main)) {
            dict = main;
        }
        if (dict) {
            p.setNewRef(PyRun_String(ptr, Py_single_input, dict, dict));
        }
emit executeProgress(75);

        if (p) {
            //result = PythonQtConv::PyObjToQVariant(p);
        } else {
            handleError();
        }

emit executeProgress(100);
    }

    bool PythonWorker::handleError()
    {
      bool flag = false;
      if (PyErr_Occurred()) {

        if (PyErr_ExceptionMatches(PyExc_SystemExit)) {
              int exitcode = custom_system_exit_exception_handler();
              Q_EMIT PythonEngine::instance().systemExitExceptionRaised(exitcode);
          }
        else
          {
          // currently we just print the error and the stderr handler parses the errors
          PyErr_Print();

          /*
          // EXTRA: the format of the ptype and ptraceback is not really documented, so I use PyErr_Print() above
          PyObject *ptype;
          PyObject *pvalue;
          PyObject *ptraceback;
          PyErr_Fetch( &ptype, &pvalue, &ptraceback);

            Py_XDECREF(ptype);
            Py_XDECREF(pvalue);
            Py_XDECREF(ptraceback);
          */
          PyErr_Clear();
          }
        flag = true;
      }
      return flag;
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
