#pragma once

#include <QMainWindow>

class QAction;
class QDockWidget;

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
        void about();
        void openPreferences();

    private:
        void createStatusBar();
        void retranslateUi();
        MainWidget *const mainWidget() const;

        QAction *_openAction;
        QAction *_saveAction;
        QAction *_saveAsAction;
        QAction *_exitAction;
        QAction *_preferencesAction;
        QAction *_aboutAction;
        QAction *_fileAction;
        QAction *_optionsAction;
        QAction *_viewAction;
        QAction *_helpAction;
        QAction *_explorerAction;
        QAction *_logsAction;
        ExplorerTreeView *_exp;
        QDockWidget *_expDock;
        QDockWidget *_logDock;
    };
}
