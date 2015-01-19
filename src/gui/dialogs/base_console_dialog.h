#pragma once

#include <QDialog>

class QProgressBar;

namespace fastoredis
{
    class FastoEditorShell;
    class FastoEditor;

    class BaseConsoleDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        enum
        {
            height = 480,
            width = 640
        };

        BaseConsoleDialog(const QString& filePath, QWidget* parent, const QIcon& icon, bool isExecuteEnabled, FastoEditorShell* shell);
        ~BaseConsoleDialog();

    protected Q_SLOTS:
        void loadFromFile();
        bool loadFromFile(const QString& path);
        void saveToFileAs();
        void saveToFile();

    private Q_SLOTS:
        void execute();
        void executeWithArgs();
        void stop();

        void executeProgressChanged(int val);
        void execute(const QString& script, const QStringList &args);
        void loadAndInstallFile();

    protected:
        virtual void changeEvent(QEvent* );

    private:
        virtual void retranslateUi();
        virtual void loadAndInstallFileImpl(const QString& path) = 0;
        virtual void executeImpl(const QString& script, const QStringList &args) = 0;
        virtual void stopImpl() = 0;

    protected:
        FastoEditorShell* shell_;
        FastoEditor* output_;

        QString filePath_;
        QProgressBar* workProgressBar_;
    };
}
