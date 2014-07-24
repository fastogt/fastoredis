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
        void finishedExecute(const EventsInfo::ExecuteInfoResponce& res);

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

    private:
        void syncConnectionActions();

        const IServerPtr _server;
        QAction *_executeAction;
        QAction *_connectAction;
        QAction *_disConnectAction;
        QAction *_loadAction;
        QAction *_saveAction;
        QAction *_saveAsAction;


        IconLabel *_serverName;
        RedisShell *_input;
        QProgressBar *_workProgressBar;
        QString _filePath;
    };
}
