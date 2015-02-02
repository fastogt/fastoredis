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
            min_height = 480,
            min_width = 640
        };

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
        BaseConsoleDialog(const QString& filePath, QWidget* parent, const QIcon& icon, bool isExecuteEnabled, FastoEditorShell* shell, const QString& fileExtension);

        virtual void changeEvent(QEvent* );
        virtual void retranslateUi();

    private:        
        virtual void loadAndInstallFileImpl(const QString& path) = 0;
        virtual void executeImpl(const QString& script, const QStringList &args) = 0;
        virtual void stopImpl() = 0;

    protected:
        FastoEditorShell* shell_;
        FastoEditor* output_;

        QString filePath_;
        QProgressBar* workProgressBar_;
        const QString fileExtension_;
    };
}
