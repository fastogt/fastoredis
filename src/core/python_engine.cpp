#include "core/python_engine.h"

#include <QThread>
#include <QApplication>

#include "common/file_system.h"
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

    PythonWorker::PythonWorker()
        : stop_(false)
    {

    }

    PythonWorker::~PythonWorker()
    {
#ifdef PYTHON_ENABLED

#endif
    }

    void PythonWorker::stop()
    {
        stop_ = true;
    }

    void PythonWorker::init()
    {
#ifdef PYTHON_ENABLED
        // add our own python object types for redirection of stdout
        if (PyType_Ready(&PythonQtStdOutRedirectType) < 0) {
            std::cerr << "could not initialize PythonQtStdOutRedirectType" << ", in " << __FILE__ << ":" << __LINE__ << std::endl;
        }
        Py_INCREF(&PythonQtStdOutRedirectType);

        PythonQtObjectPtr sys;
        sys.setNewRef(PyImport_ImportModule("sys"));

        //os.path.abspath(os.path.dirname(sys.argv[0]))

        /*PythonQtObjectPtr os;
        os.setNewRef(PyImport_ImportModule("os"));
        PyObject* pathObj = PyObject_GetAttrString(os, "path");
        PyObject* chdirFunc = PyObject_GetAttrString(pathObj, "abspath");
        if(chdirFunc && PyCallable_Check(chdirFunc)){
            PyObject* pArgs = PyTuple_New(1);
            std::string app = common::convertToString(QCoreApplication::applicationDirPath());
            PyObject* pdir = PyString_FromString(app.c_str());
            PyTuple_SetItem(pArgs, 0, pdir);
            PyObject_CallObject(chdirFunc, pArgs);
            Py_DECREF(pArgs);
            Py_DECREF(chdirFunc);
        }*/

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
#endif
    }

    void PythonWorker::customEvent(QEvent* event)
    {
        using namespace Events;
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

    namespace
    {
        template<typename char_type>
        char_type** toPythonArgs(const std::vector<std::basic_string<char_type> >& args)
        {
        #if 0
            argc = args.size() +1;
            char_type** argv = (char_type**)calloc(argc, sizeof(char_type*));
            std::basic_string<char_type> argv0 = common::convertToString(QCoreApplication::applicationFilePath());
            argv[0] = (char_type*)calloc(argv0.size(), sizeof(char_type));
            memcpy(argv[0], argv0.c_str(), argv0.size());

            for(int i = 0; i < args.size(); ++i){
                std::basic_string<char_type> argvi = args[i];
                argv[i+1] = (char_type*)calloc(argvi.size(), sizeof(char_type));
                memcpy(argv[i+1], argvi.c_str(), argvi.size());
            }

            return argv;
        #else
            int argc = args.size();
            char_type** argv = (char_type**)calloc(argc, sizeof(char_type*));
            for(int i = 0; i < args.size(); ++i){
                std::basic_string<char_type> argvi = args[i];
                argv[i] = (char_type*)calloc(argvi.size(), sizeof(char_type));
                memcpy(argv[i], argvi.c_str(), argvi.size());
            }

            return argv;
        #endif
        }
    }

    void PythonWorker::executeImpl(const std::string& script, const std::vector<std::string>& args)
    {
#ifdef PYTHON_ENABLED
emit executeProgress(0);

        const char* ptr = script.c_str();
        if(!ptr){
            return;
emit executeProgress(100);
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
            int argc = args.size();
            if(argc){
    #ifndef PY3K
        typedef char char_type;
    #else
        typedef wchar_t char_type;
    #endif
                char_type** argv = toPythonArgs(args);

                PySys_SetArgv(argc, argv);
                p.setNewRef(PyRun_String(ptr, Py_single_input, dict, dict));

                for(int i = 0; i < argc; ++i){
                    free(argv[i]);
                }
                free(argv);
            }
            else{
                p.setNewRef(PyRun_String(ptr, Py_single_input, dict, dict));
            }

        }
emit executeProgress(75);

        if (p) {
            //result = PythonQtConv::PyObjToQVariant(p);
        }
        else {
            handleError();
        }

emit executeProgress(100);
#endif
    }

    void PythonWorker::executeScriptImpl(const std::string& path, const std::vector<std::string>& args)
    {
#ifdef PYTHON_ENABLED
emit executeProgress(0);
        const char* ptrPath = path.c_str();
        if(!ptrPath){
            emit executeProgress(100);
            return;
        }

        PythonQtObjectPtr main = getMainModule();
        PyObject* dict = NULL;
        if (PyModule_Check(main)) {
            dict = PyModule_GetDict(main);
        }
        else if (PyDict_Check(main)) {
            dict = main;
        }

        dict = PyDict_Copy(dict);

        if (!dict) {
            emit executeProgress(100);
            return;
        }

        FILE* file = fopen(ptrPath, "r");
        if(file){            
#ifndef PY3K
            typedef char char_type;
#else
            typedef wchar_t char_type;
#endif
            int argc = args.size() + 1;
            std::vector<std::string> argssc;
            argssc.push_back(ptrPath);
            for(int i = 0; i < args.size(); ++i){
                argssc.push_back(args[i]);
            }

            char_type** argv = toPythonArgs(argssc);
            PySys_SetArgv(argc, argv);
            PythonQtObjectPtr p;

            if (PyDict_GetItemString(dict, "__file__") == NULL) {
                PyObject *f = PyString_FromString(ptrPath);
                if (f == NULL) {
                    goto done;
                }
                if (PyDict_SetItemString(dict, "__file__", f) < 0) {
                    Py_DECREF(f);
                    goto done;
                }
            }

            p.setNewRef(PyRun_File(file, ptrPath, Py_file_input, dict, dict));
            if (p) {
                //result = PythonQtConv::PyObjToQVariant(p);
            }
            else {
                handleError();
            }

done:
            Py_DECREF(dict);
            for(int i = 0; i < argc; ++i){
                free(argv[i]);
            }
            free(argv);
            fclose(file);
        }
emit executeProgress(100);
#endif
    }

    void PythonWorker::execute(const QString& script, const QStringList &args)
    {
        std::vector<std::string> sargs;
        for(QStringList::const_iterator it = args.begin(); it < args.end(); ++it){
            QString val = *it;
            sargs.push_back(common::convertToString(val));
        }

        EventsInfo::ExecuteInfoRequest req(common::convertToString(script), sargs);
        QEvent *ev = new Events::ExecuteRequestEvent(this, req);
        qApp->postEvent(this, ev);
    }

    void PythonWorker::executeScript(const QString& path, const QStringList& args)
    {
        std::vector<std::string> sargs;
        for(QStringList::const_iterator it = args.begin(); it < args.end(); ++it){
            QString val = *it;
            sargs.push_back(common::convertToString(val));
        }

        EventsInfo::ExecuteScriptInfoRequest req(common::convertToString(path), sargs);
        QEvent *ev = new Events::ExecuteScriptRequestEvent(this, req);
        qApp->postEvent(this, ev);
    }

    bool PythonWorker::handleError()
    {
        bool flag = false;
#ifdef PYTHON_ENABLED
        if (PyErr_Occurred()) {
            if (PyErr_ExceptionMatches(PyExc_SystemExit)) {
                int exitcode = custom_system_exit_exception_handler();
                Q_EMIT systemExitExceptionRaised(exitcode);
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
#endif
        return flag;
    }

    const char* PythonEngine::version()
    {
#ifdef PYTHON_ENABLED
        return PY_VERSION;
#else
        return "Unknown";
#endif
    }

    PythonEngine::PythonEngine()
    {
#ifdef PYTHON_ENABLED
#ifndef PY3K
        Py_SetProgramName(PROJECT_NAME);  /* optional but recommended */
#else
        Py_SetProgramName(WCHAR_PROJECT_NAME);  /* optional but recommended */
#endif
        Py_Initialize();
#endif
    }

    std::string PythonEngine::execPath() const
    {
        return SettingsManager::instance().pythonExecPath();
    }

    PythonWorker* PythonEngine::createWorker()
    {
        PythonWorker* worker = new PythonWorker;
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        VERIFY(QObject::connect(thread, SIGNAL(started()), worker, SLOT(init())));
        thread->start();

        return worker;
    }

    PythonEngine::~PythonEngine()
    {
#ifdef PYTHON_ENABLED
        //Py_Finalize();
#endif
    }
}
