#include "gui/dialogs/lua_console_dialog.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "shell/python_shell.h"
#include "core/lua_engine.h"

namespace fastoredis
{
    LuaConsoleDialog::LuaConsoleDialog(const QString& filePath, QWidget* parent)
        : BaseConsoleDialog(filePath, parent, GuiFactory::instance().luaIcon(), true, NULL), worker_(NULL)
    {
        using namespace translations;
        worker_ = LuaEngine::instance().createWorker();

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
