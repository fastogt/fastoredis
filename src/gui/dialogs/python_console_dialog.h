#pragma once

#include "gui/dialogs/base_console_dialog.h"

namespace fastoredis
{
    class PythonShell;
    class PythonWorker;
    class FastoEditor;

    class PythonConsoleDialog
            : public BaseConsoleDialog
    {
        Q_OBJECT
    public:
        PythonConsoleDialog(const QString& filePath = QString(), QWidget* parent = 0);
        ~PythonConsoleDialog();

    protected:
        virtual void retranslateUi();

    private:
        virtual void loadAndInstallFileImpl(const QString& path);
        virtual void executeImpl(const QString& script, const QStringList &args);
        virtual void stopImpl();

        PythonWorker* worker_;
    };
}
