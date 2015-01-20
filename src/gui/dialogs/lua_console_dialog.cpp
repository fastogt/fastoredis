#include "gui/dialogs/lua_console_dialog.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "shell/lua_shell.h"
#include "core/lua_engine.h"

namespace fastoredis
{
    namespace
    {
        FastoEditorShell* shellCreate()
        {
            if(LuaEngine::instance().hasModule("redis")){
                return new RedisLuaShell;
            }
            else{
                return new LuaShell;
            }
        }
    }

    LuaConsoleDialog::LuaConsoleDialog(const QString& filePath, QWidget* parent)
        : BaseConsoleDialog(filePath, parent, GuiFactory::instance().luaIcon(), true, shellCreate()), worker_(NULL)
    {
        using namespace translations;
        worker_ = LuaEngine::instance().createWorker();

        VERIFY(connect(worker_, SIGNAL(luaStdOut(const QString&)), output_, SLOT(append(const QString&)), Qt::QueuedConnection));
        VERIFY(connect(worker_, SIGNAL(luaStdErr(const QString&)), output_, SLOT(append(const QString&)), Qt::QueuedConnection));
        VERIFY(connect(worker_, SIGNAL(executeProgress(int)), this, SLOT(executeProgressChanged(int))));

        retranslateUi();
    }

    LuaConsoleDialog::~LuaConsoleDialog()
    {
        delete worker_;
    }

    void LuaConsoleDialog::retranslateUi()
    {
        using namespace translations;
        setWindowTitle(trLuaConsole);
        BaseConsoleDialog::retranslateUi();
    }

    void LuaConsoleDialog::loadAndInstallFileImpl(const QString& path)
    {
        if(loadFromFile(path)){
            worker_->executeScript(filePath_, QStringList() << "install");
        }
    }

    void LuaConsoleDialog::executeImpl(const QString& script, const QStringList &args)
    {
        worker_->execute(script, args);
    }

    void LuaConsoleDialog::stopImpl()
    {
    }
}
