#include "gui/dialogs/python_console_dialog.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "shell/python_shell.h"
#include "core/python_engine.h"

namespace fastoredis
{
    namespace
    {
        FastoEditorShell* shellCreate()
        {
            if(PythonEngine::instance().hasModule("redis-py")){
                return new RedisPyShell;
            }
            else{
                return new PythonShell;
            }
        }
    }

    PythonConsoleDialog::PythonConsoleDialog(const QString& filePath, QWidget* parent)
        : BaseConsoleDialog(filePath, parent, GuiFactory::instance().pythonIcon(), isPythonEnabled(), shellCreate()), worker_(NULL)
    {
        using namespace translations;
        worker_ = PythonEngine::instance().createWorker();

        VERIFY(connect(worker_, SIGNAL(pythonStdOut(const QString&)), output_, SLOT(append(const QString&)), Qt::QueuedConnection));
        VERIFY(connect(worker_, SIGNAL(pythonStdErr(const QString&)), output_, SLOT(append(const QString&)), Qt::QueuedConnection));
        VERIFY(connect(worker_, SIGNAL(executeProgress(int)), this, SLOT(executeProgressChanged(int))));

        retranslateUi();
    }

    PythonConsoleDialog::~PythonConsoleDialog()
    {
        delete worker_;
    }

    void PythonConsoleDialog::retranslateUi()
    {
        using namespace translations;
        setWindowTitle(trPythonConsole);
    }

    void PythonConsoleDialog::loadAndInstallFileImpl(const QString& path)
    {
        if(loadFromFile(path)){
            worker_->executeScript(filePath_, QStringList() << "install");
        }
    }

    void PythonConsoleDialog::executeImpl(const QString& script, const QStringList &args)
    {
        worker_->execute(script, args);
    }

    void PythonConsoleDialog::stopImpl()
    {
    }
}
