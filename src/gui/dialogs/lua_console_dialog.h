#pragma once

#include "gui/dialogs/base_console_dialog.h"

namespace fastoredis
{
    class LuaWorker;

    class LuaConsoleDialog
            : public BaseConsoleDialog
    {
        Q_OBJECT
    public:
        LuaConsoleDialog(const QString& filePath = QString(), QWidget* parent = 0);
        ~LuaConsoleDialog();

    protected:
        virtual void retranslateUi();

    private:
        virtual void loadAndInstallFileImpl(const QString& path);
        virtual void executeImpl(const QString& script, const QStringList& args);
        virtual void stopImpl();

        LuaWorker* worker_;
    };
}
