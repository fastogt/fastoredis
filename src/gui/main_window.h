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
        virtual void changeEvent(QEvent* );
        virtual void showEvent(QShowEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);

    private Q_SLOTS:
        void open();
        void save();
        void saveAs();
        void about();
        void openPreferences();
        void checkUpdate();

        void versionAvailible(bool succesResult, const QString& version);
    private:
        void createStatusBar();
        void retranslateUi();
        MainWidget* const mainWidget() const;

        QAction* openAction_;
        QAction* saveAction_;
        QAction* saveAsAction_;
        QAction* exitAction_;
        QAction* preferencesAction_;
        QAction* fullScreanAction_;
        QAction* windowAction_;
        QAction* aboutAction_;
        QAction* fileAction_;
        QAction* optionsAction_;
        QAction* checkUpdateAction_;
        QAction* viewAction_;
        QAction* helpAction_;
        QAction* explorerAction_;
        QAction* logsAction_;
        ExplorerTreeView* exp_;
        QDockWidget* expDock_;
        QDockWidget* logDock_;
        bool isCheckedInSession_;
    };

    class UpdateChecker
            : public QObject
    {
        Q_OBJECT
    public:
        UpdateChecker(QObject* parent = 0);

    Q_SIGNALS:
        void versionAvailibled(bool succesResult, const QString& version);

    public Q_SLOTS:
        void routine();
    };
}
