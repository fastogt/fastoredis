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
            if(LuaEngine::instance().hasModule("redis-lua")){
                return new RedisLuaShell;
            }
            else{
                return new LuaShell;
            }
        }
    }

    LuaConsoleDialog::LuaConsoleDialog(const QString& filePath, QWidget* parent)
        : BaseConsoleDialog(filePath, parent, GuiFactory::instance().luaIcon(), true, shellCreate(), "lua"), worker_(NULL)
    {
        using namespace translations;
        worker_ = LuaEngine::instance().createWorker();

        VERIFY(connect(worker_, &LuaWorker::luaStdOut, output_, &FastoEditor::append, Qt::QueuedConnection));
        VERIFY(connect(worker_, &LuaWorker::luaStdErr, output_, &FastoEditor::append, Qt::QueuedConnection));
        VERIFY(connect(worker_, &LuaWorker::executeProgress, this, &LuaConsoleDialog::executeProgressChanged));

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

    void LuaConsoleDialog::executeImpl(const QString& script, const QStringList& args)
    {
        worker_->execute(script, args);
    }

    void LuaConsoleDialog::stopImpl()
    {
    }
}
