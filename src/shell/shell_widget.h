#pragma once

/**/

#include <QWidget>

#include "core/iserver.h"
#include "core/events/events_info.h"

class QAction;
class QProgressBar;
class QToolButton;

namespace fastoredis
{
    class RedisShell;
    class IconLabel;

    class ShellWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        ShellWidget(IServerPtr server, const QString& filePath = QString(), QWidget* parent = 0);

        IServerPtr server() const;
        QString text() const;

    Q_SIGNALS:
        void startedExecute(const EventsInfo::ExecuteInfoRequest& req);
        void rootCreated(const EventsInfo::CommandRootCreatedInfo& res);
        void rootCompleated(const EventsInfo::CommandRootCompleatedInfo& res);

        void addedChild(FastoObject *child);
        void itemUpdated(FastoObject*, const QString&);

    public Q_SLOTS:
        void setText(const QString& text);

    private Q_SLOTS:
        void execute();
        void stop();
        void connectToServer();
        void disconnectFromServer();
        void loadFromFile();
        bool loadFromFile(const QString& path);
        void saveToFileAs();
        void saveToFile();

        void startConnect(const EventsInfo::ConnectInfoRequest& req);
        void finishConnect(const EventsInfo::ConnectInfoResponce& res);
        void startDisconnect(const EventsInfo::DisonnectInfoRequest& req);
        void finishDisconnect(const EventsInfo::DisConnectInfoResponce& res);

        void progressChange(const EventsInfo::ProgressInfoResponce& res);

        void enterMode(const EventsInfo::EnterModeInfo& res);
        void leaveMode(const EventsInfo::LeaveModeInfo& res);

    private:
        void syncConnectionActions();

        const IServerPtr server_;
        QAction* executeAction_;
        QAction* connectAction_;
        QAction* disConnectAction_;
        QAction* loadAction_;
        QAction* saveAction_;
        QAction* saveAsAction_;

        IconLabel* serverName_;
        RedisShell* input_;
        QProgressBar* workProgressBar_;
        IconLabel* connectionMode_;
        QString filePath_;
    };
}
