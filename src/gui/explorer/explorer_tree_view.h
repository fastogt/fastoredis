#pragma once

#include <QTreeView>

class QAction;

#include "core/iserver.h"

namespace fastoredis
{
    class ExplorerTreeView
            : public QTreeView
    {
        Q_OBJECT

    public:
        ExplorerTreeView(QWidget* parent);

    Q_SIGNALS:
        void openedConsole(IServerSPtr server);

    public Q_SLOTS:
        void addServer(IServerSPtr server);
        void removeServer(IServerSPtr server);

    private Q_SLOTS:
        void showContextMenu(const QPoint& point);
        void connectDisconnectToServer();
        void openConsole();
        void loadDatabases();
        void loadContentDb();
        void openInfoServerDialog();
        void openPropertyServerDialog();
        void openHistoryServerDialog();
        void closeConnection();

        void backupServer();
        void importServer();
        void shutdownServer();

        void startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest& req);
        void finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce& res);

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();
        QModelIndex selectedIndex() const;
        QModelIndexList selectedIndexes() const;

        QAction* connectAction_;
        QAction* openConsoleAction_;
        QAction* loadDatabaseAction_;
        QAction* loadContentAction_;
        QAction* infoServerAction_;
        QAction* propertyServerAction_;
        QAction* historyServerAction_;
        QAction* closeAction_;
        QAction* importAction_;
        QAction* backupAction_;
        QAction* shutdownAction_;
    };
}
