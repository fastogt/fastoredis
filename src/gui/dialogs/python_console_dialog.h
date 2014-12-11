#pragma once

/**/

#include <QDialog>

class QProgressBar;

namespace fastoredis
{
    class PythonShell;
    class PythonWorker;
    class FastoEditor;

    class PythonConsoleDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        enum
        {
            height = 480,
            width = 640
        };

        PythonConsoleDialog(const QString& filePath = QString(), QWidget* parent = 0);
        ~PythonConsoleDialog();

    private Q_SLOTS:
        void execute();
        void executeWithArgs();
        void stop();
        void loadFromFile();
        bool loadFromFile(const QString& path);
        void saveToFileAs();
        void saveToFile();
        void executeProgressChanged(int val);
        void execute(const QString& script, const QStringList &args);

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();

        PythonShell* shell_;
        FastoEditor* output_;
        QString filePath_;
        QProgressBar* workProgressBar_;
        PythonWorker* worker_;
    };
}
