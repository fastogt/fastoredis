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
        void openInfoServerDialog();
        void openPropertyServerDialog();

        void startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req);
        void finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res);

    protected:
        virtual void changeEvent(QEvent *);

    private:
        void retranslateUi();
        QModelIndex selectedIndex() const;
        QModelIndexList selectedIndexes() const;

        QAction *connectAction_;
        QAction *openConsoleAction_;
        QAction *loadDatabaseAction_;
        QAction *loadContentAction_;
        QAction *infoServerAction_;
        QAction *propertyServerAction_;
    };
}
