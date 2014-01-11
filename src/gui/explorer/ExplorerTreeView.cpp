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
#include "common/qt/converter_patterns.h"

namespace fastoredis
{
    ExplorerTreeView::ExplorerTreeView(QWidget *parent)
        : QTreeView(parent)
    {
        setModel(new ExplorerTreeModel(this));
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&))));

        _connectAction = new QAction(this);
        VERIFY(connect(_connectAction, SIGNAL(triggered()), SLOT(connectToServer())));
        _openConsoleAction = new QAction(this);
        VERIFY(connect(_openConsoleAction, SIGNAL(triggered()), SLOT(openConsole())));
        _loadDatabaseAction = new QAction(this);
        VERIFY(connect(_loadDatabaseAction, SIGNAL(triggered()), SLOT(loadDatabases())));

        _loadContent = new QAction(this);
        VERIFY(connect(_loadContent, SIGNAL(triggered()), SLOT(loadContentDb())));
        retranslateUi();
    }

    void ExplorerTreeView::showContextMenu(const QPoint &point)
    {
        QPoint menuPoint = mapToGlobal(point);
        menuPoint.setY(menuPoint.y() + header()->height());

        QModelIndex sel = selectedIndex();
        if(sel.isValid()){            
            IExplorerTreeItem *node = common::utils_qt::item<IExplorerTreeItem*>(sel);
            if(node->type() == IExplorerTreeItem::Server){
                QMenu menu(this);
                menu.addAction(_connectAction);
                menu.addAction(_openConsoleAction);
                menu.addAction(_loadDatabaseAction);
                menu.exec(menuPoint);
            }
            else if(node->type() == IExplorerTreeItem::Database){
                QMenu menu(this);
                menu.addAction(_loadContent);
                menu.exec(menuPoint);
            }
        }
    }

    void ExplorerTreeView::connectToServer()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerPtr server = node->server();
                if(server->isConnected()){
                    server->disconnect();
                }
                else{
                    server->connect();
                }
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

    void ExplorerTreeView::loadDatabases()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                node->server()->loadDatabases();
            }
        }
    }

    void ExplorerTreeView::loadContentDb()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerDatabaseItem *node = common::utils_qt::item<ExplorerDatabaseItem*>(sel);
            if(node){
                node->loadContent();
            }
        }
    }

    void ExplorerTreeView::addServer(const IServerPtr &server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        VERIFY(connect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
//        VERIFY(connect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
//        VERIFY(connect(server.get(), SIGNAL(finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

        mod->addServer(server);
    }

    void ExplorerTreeView::removeServer(const IServerPtr &server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        VERIFY(disconnect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
//        VERIFY(disconnect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
//        VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

        mod->removeServer(server);
    }

    QModelIndex ExplorerTreeView::selectedIndex() const
    {
        QModelIndexList indexses = selectionModel()->selectedRows();

        if (indexses.count() != 1)
            return QModelIndex();

        return indexses[0];
    }

    void ExplorerTreeView::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QTreeView::changeEvent(e);
    }

    void ExplorerTreeView::retranslateUi()
    {
        _connectAction->setText(tr("Connect/Disconnect"));
        _openConsoleAction->setText(tr("Open console"));
        _loadDatabaseAction->setText(tr("Load databases"));
        _loadContent->setText(tr("Load content of database"));
    }

    void ExplorerTreeView::startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req)
    {

    }

    void ExplorerTreeView::finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res)
    {
        const error::ErrorInfo &er = res.errorInfo();
        if(!er.isError()){
            IServer *serv = qobject_cast<IServer *>(sender());
            DCHECK(serv);
            EventsInfo::LoadDatabasesInfoResponce::database_info_cont_type dbs = res.databases_;
            ExplorerTreeModel *mod = qobject_cast<ExplorerTreeModel *>(model());
            DCHECK(mod);
            for(int i = 0; i < dbs.size(); ++i){
                DataBaseInfo db = dbs[i];
                mod->addDatabase(serv, db);
            }
        }
    }

    QModelIndexList ExplorerTreeView::selectedIndexes() const
    {
        return selectionModel()->selectedRows();
    }
}
