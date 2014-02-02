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

#include "gui/dialogs/InfoServerDialog.h"
#include "gui/dialogs/PropertyServerDialog.h"

#include "common/qt/convert_string.h"

namespace fastoredis
{
    ExplorerTreeView::ExplorerTreeView(QWidget *parent)
        : QTreeView(parent)
    {
        setModel(new ExplorerTreeModel(this));
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&))));

        connectAction_ = new QAction(this);
        VERIFY(connect(connectAction_, SIGNAL(triggered()), SLOT(connectToServer())));
        openConsoleAction_ = new QAction(this);
        VERIFY(connect(openConsoleAction_, SIGNAL(triggered()), SLOT(openConsole())));
        loadDatabaseAction_ = new QAction(this);
        VERIFY(connect(loadDatabaseAction_, SIGNAL(triggered()), SLOT(loadDatabases())));

        loadContentAction_ = new QAction(this);
        VERIFY(connect(loadContentAction_, SIGNAL(triggered()), SLOT(loadContentDb())));

        infoServerAction_ = new QAction(this);
        VERIFY(connect(infoServerAction_, SIGNAL(triggered()), SLOT(openInfoServerDialog())));

        propertyServerAction_ = new QAction(this);
        VERIFY(connect(propertyServerAction_, SIGNAL(triggered()), SLOT(openPropertyServerDialog())));

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
                menu.addAction(connectAction_);
                menu.addAction(openConsoleAction_);
                menu.addAction(loadDatabaseAction_);
                menu.addAction(infoServerAction_);
                menu.addAction(propertyServerAction_);
                menu.exec(menuPoint);
            }
            else if(node->type() == IExplorerTreeItem::Database){
                QMenu menu(this);
                menu.addAction(loadContentAction_);
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
//      VERIFY(connect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
//      VERIFY(connect(server.get(), SIGNAL(finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

        mod->addServer(server);
    }

    void ExplorerTreeView::removeServer(const IServerPtr &server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        VERIFY(disconnect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
//      VERIFY(disconnect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
//      VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

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
        connectAction_->setText(tr("Connect"));
        openConsoleAction_->setText(tr("Open console"));
        loadDatabaseAction_->setText(tr("Load databases"));
        loadContentAction_->setText(tr("Load content of database"));
        infoServerAction_->setText(tr("Info"));
        propertyServerAction_->setText(tr("Property"));
    }

    void ExplorerTreeView::startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &req)
    {

    }

    void ExplorerTreeView::finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &res)
    {
        const common::ErrorValue &er = res.errorInfo();
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

    void ExplorerTreeView::openInfoServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerPtr server = node->server();
                InfoServerDialog infDialog(server->name() + " info", server->connectionType(), this);
                VERIFY(connect(server.get(), SIGNAL(startedServerInfo(const EventsInfo::ServerInfoRequest &)), &infDialog, SLOT(startServerInfo(const EventsInfo::ServerInfoRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedServerInfo(const EventsInfo::ServerInfoResponce &)), &infDialog, SLOT(finishServerInfo(const EventsInfo::ServerInfoResponce &))));
                VERIFY(connect(&infDialog, SIGNAL(showed()), server.get(), SLOT(serverInfo())));
                infDialog.exec();
            }
        }
    }

    void ExplorerTreeView::openPropertyServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerPtr server = node->server();
                PropertyServerDialog infDialog(server->name() + " properties", server->connectionType(), this);
                VERIFY(connect(server.get(), SIGNAL(startedServerProperty(const EventsInfo::ServerPropertyRequest &)), &infDialog, SLOT(startServerProperty(const EventsInfo::ServerPropertyRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedServerProperty(const EventsInfo::ServerPropertyResponce &)), &infDialog, SLOT(finishServerProperty(const EventsInfo::ServerPropertyResponce &))));
                VERIFY(connect(server.get(), SIGNAL(startedServerChangeProperty(const EventsInfo::ServerPropertyChangeRequest &)), &infDialog, SLOT(startServerChangeProperty(const EventsInfo::ServerPropertyChangeRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedServerChangeProperty(const EventsInfo::ServerPropertyChangeResponce &)), &infDialog, SLOT(finishServerChangeProperty(const EventsInfo::ServerPropertyChangeResponce &))));
                VERIFY(connect(&infDialog, SIGNAL(changedProperty(const PropertyType&)), server.get(), SLOT(changeProperty(const PropertyType&))));
                VERIFY(connect(&infDialog, SIGNAL(showed()), server.get(), SLOT(serverProperty())));
                infDialog.exec();
            }
        }
    }
}
