#pragma once

#include <QMainWindow>

class QAction;

namespace fastoredis
{
    class MainWidget;
    class ExplorerTreeView;

    class MainWindow
            : public QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow();

    protected:
        virtual void changeEvent(QEvent *);

    private Q_SLOTS:
        void open();
        void save();
        void saveAs();
        void exit();
        void about();
        void openPreferences();

    private:
        void createStatusBar();
        MainWidget *const mainWidget() const;

        QAction *_openAction;
        QAction *_saveAction;
        QAction *_saveAsAction;
        ExplorerTreeView *_exp;
    };
}
