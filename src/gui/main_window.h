#pragma once

#include <QMainWindow>

class QAction;
class QDockWidget;

#ifdef OS_ANDROID
class QGestureEvent;
class QSwipeGesture;
class QTapAndHoldGesture;
#endif

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
        ~MainWindow();

    protected:
        virtual void changeEvent(QEvent* ev);
        virtual void showEvent(QShowEvent* ev);

    private Q_SLOTS:
        void open();
        void save();
        void saveAs();
        void about();
        void openPreferences();
        void checkUpdate();
        void enterLeaveFullScreen();
        void openPythonConsole();
        void openLuaConsole();
        void openEncodeDecodeDialog();

        void versionAvailible(bool succesResult, const QString& version);

    protected:
#ifdef OS_ANDROID
        virtual bool event(QEvent *event);
        bool gestureEvent(QGestureEvent *event);
        void swipeTriggered(QSwipeGesture* swipeEvent);
        void tapAndHoldTriggered(QTapAndHoldGesture* tapEvent);
#endif

    private:
        void createStatusBar();
        void retranslateUi();

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
        QAction* toolsAction_;
        QAction* pythonConsoleAction_;
        QAction* luaConsoleAction_;
        QAction* encodeDecodeDialogAction_;
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
