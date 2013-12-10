#pragma once

#include <QTreeView>
QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include "global/global.h"
#include "core/IServer.h"

namespace fastoredis
{
    class ExplorerTreeView
            : public QTreeView
    {
        Q_OBJECT

    public:
        typedef QTreeView base_class;
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

    private:        
        QModelIndex selectedIndex() const;
        QModelIndexList selectedIndexes() const;
        QAction *_connectAction;
        QAction *_openConsoleAction;
    };
}
