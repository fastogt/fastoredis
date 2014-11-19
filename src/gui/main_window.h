#pragma once

/**/

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
        void checkUpdate();

    private:
        void createStatusBar();
        void retranslateUi();
        MainWidget *const mainWidget() const;

        QAction *openAction_;
        QAction *saveAction_;
        QAction *saveAsAction_;
        QAction *exitAction_;
        QAction *preferencesAction_;
        QAction *aboutAction_;
        QAction *fileAction_;
        QAction *optionsAction_;
        QAction *checkUpdateAction_;
        QAction *viewAction_;
        QAction *helpAction_;
        QAction *explorerAction_;
        QAction *logsAction_;
        ExplorerTreeView *exp_;
        QDockWidget *expDock_;
        QDockWidget *logDock_;
    };
}
