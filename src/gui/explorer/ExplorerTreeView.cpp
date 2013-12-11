#include "gui/explorer/ExplorerTreeView.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QTreeView>
#include <QAction>
#include <QHeaderView>
#include <QMenu>

#include "gui/explorer/ExplorerTreeModel.h"
#include "gui/explorer/ExplorerTreeModel.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    ExplorerTreeView::ExplorerTreeView(QWidget *parent)
        : base_class(parent)
    {
        setModel(new ExplorerTreeModel(this));
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&))));

        _connectAction = new QAction("Connect", this);
        VERIFY(connect(_connectAction, SIGNAL(triggered()), SLOT(connectToServer())));
        _openConsoleAction = new QAction("Open console", this);
        VERIFY(connect(_openConsoleAction, SIGNAL(triggered()), SLOT(openConsole())));
    }

    void ExplorerTreeView::showContextMenu(const QPoint &point)
    {
        QPoint menuPoint = mapToGlobal(point);
        menuPoint.setY(menuPoint.y() + header()->height());

        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            QMenu menu(this);

            menu.addAction(_connectAction);
            menu.addAction(_openConsoleAction);
            menu.exec(menuPoint);
        }
    }

    void ExplorerTreeView::connectToServer()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                node->server()->connect();
            }
        }
    }

    void ExplorerTreeView::openConsole()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                emit openedConsole(node->server());
            }
        }
    }

    void ExplorerTreeView::addServer(const IServerPtr &server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        VERIFY(connect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

        mod->addServer(server);
    }

    void ExplorerTreeView::removeServer(const IServerPtr &server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        mod->removeServer(server);
    }

    QModelIndex ExplorerTreeView::selectedIndex() const
    {
        QModelIndexList indexses = selectionModel()->selectedRows();

        if (indexses.count() != 1)
            return QModelIndex();

        return indexses[0];
    }

    void ExplorerTreeView::startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req)
    {

    }

    void ExplorerTreeView::finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res)
    {

    }

    QModelIndexList ExplorerTreeView::selectedIndexes() const
    {
        return selectionModel()->selectedRows();
    }
}
