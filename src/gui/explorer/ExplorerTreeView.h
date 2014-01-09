#pragma once

#include <QTreeView>

class QAction;

#include "global/global.h"
#include "core/IServer.h"

namespace fastoredis
{
    class ExplorerTreeView
            : public QTreeView
    {
        Q_OBJECT

    public:
        ExplorerTreeView(QWidget *parent);

    Q_SIGNALS:
        void openedConsole(const IServerPtr &server);

    public Q_SLOTS:
        void addServer(const IServerPtr &server);
        void removeServer(const IServerPtr &server);

    private Q_SLOTS:
        void showContextMenu(const QPoint &point);
        void connectToServer();
        void openConsole();
        void loadDatabases();
        void loadContentDb();

        void startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req);
        void finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res);

    protected:
        virtual void changeEvent(QEvent *);

    private:
        void retranslateUi();
        QModelIndex selectedIndex() const;
        QModelIndexList selectedIndexes() const;

        QAction *_connectAction;
        QAction *_openConsoleAction;
        QAction *_loadDatabaseAction;
        QAction *_loadContent;
    };
}
