#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

namespace fastoredis
{
    class MainWindow
            : public QMainWindow
    {
        Q_OBJECT
    public:
        typedef QMainWindow base_class;
        MainWindow();

    private Q_SLOTS:
        void open();
        void save();
        void saveAs();
        void exit();
        void about();
        void openPreferences();

    private:
        void createStatusBar();

        QAction *_openAction;
        QAction *_saveAction;
        QAction *_saveAsAction;
    };
}
