#include "gui/explorer/explorer_tree_view.h"

#include <QMenu>
#include <QHeaderView>
#include <QAction>

#include "gui/explorer/explorer_tree_model.h"

#include "gui/dialogs/info_server_dialog.h"
#include "gui/dialogs/property_server_dialog.h"
#include "gui/dialogs/history_server_dialog.h"

#include "common/qt/convert_string.h"

#include "translations/global.h"

namespace fastoredis
{
    ExplorerTreeView::ExplorerTreeView(QWidget* parent)
        : QTreeView(parent)
    {
        setModel(new ExplorerTreeModel(this));

        setSelectionBehavior(QAbstractItemView::SelectRows);
        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&))));

        connectAction_ = new QAction(this);
        VERIFY(connect(connectAction_, SIGNAL(triggered()), SLOT(connectDisconnectToServer())));
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

        historyServerAction_ = new QAction(this);
        VERIFY(connect(historyServerAction_, SIGNAL(triggered()), SLOT(openHistoryServerDialog())));

        closeAction_ = new QAction(this);
        VERIFY(connect(closeAction_, SIGNAL(triggered()), SLOT(closeConnection())));

        retranslateUi();
    }

    void ExplorerTreeView::showContextMenu(const QPoint& point)
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

                IServerSPtr server = node->server();
                bool isCon = server->isConnected();

                loadDatabaseAction_->setEnabled(isCon);
                menu.addAction(loadDatabaseAction_);
                infoServerAction_->setEnabled(isCon);
                menu.addAction(infoServerAction_);
                propertyServerAction_->setEnabled(isCon);
                menu.addAction(propertyServerAction_);

                menu.addAction(historyServerAction_);
                menu.addAction(closeAction_);
                menu.exec(menuPoint);
            }
            else if(node->type() == IExplorerTreeItem::Database){
                QMenu menu(this);
                menu.addAction(loadContentAction_);
                menu.exec(menuPoint);
            }
        }
    }

    void ExplorerTreeView::connectDisconnectToServer()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerSPtr server = node->server();
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

    void ExplorerTreeView::addServer(IServerSPtr server)
    {
        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        VERIFY(connect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
//      VERIFY(connect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
//      VERIFY(connect(server.get(), SIGNAL(finishedLoadDataBaseContent(const EventsInfo::LoadDatabasesContentRequest &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));

        mod->addServer(server);
    }

    void ExplorerTreeView::removeServer(IServerSPtr server)
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

    void ExplorerTreeView::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QTreeView::changeEvent(e);
    }

    void ExplorerTreeView::retranslateUi()
    {
        using namespace translations;

        connectAction_->setText(tr("Connect/Disconnect"));
        openConsoleAction_->setText(trOpenConsole);
        loadDatabaseAction_->setText(trLoadDataBases);
        loadContentAction_->setText(trLoadContOfDataBases);
        infoServerAction_->setText(trInfo);
        propertyServerAction_->setText(trProperty);
        historyServerAction_->setText(trHistory);
        closeAction_->setText(trClose);
    }

    void ExplorerTreeView::startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest& req)
    {

    }

    void ExplorerTreeView::finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce& res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

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
                IServerSPtr server = node->server();
                InfoServerDialog infDialog(server->name() + " info", this);
                VERIFY(connect(server.get(), SIGNAL(startedLoadServerInfo(const EventsInfo::ServerInfoRequest &)), &infDialog, SLOT(startServerInfo(const EventsInfo::ServerInfoRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedLoadServerInfo(const EventsInfo::ServerInfoResponce &)), &infDialog, SLOT(finishServerInfo(const EventsInfo::ServerInfoResponce &))));
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
                IServerSPtr server = node->server();
                PropertyServerDialog infDialog(server->name() + " properties", server->connectionType(), this);
                VERIFY(connect(server.get(), SIGNAL(startedLoadServerProperty(const EventsInfo::ServerPropertyInfoRequest &)), &infDialog, SLOT(startServerProperty(const EventsInfo::ServerPropertyInfoRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedLoadServerProperty(const EventsInfo::ServerPropertyInfoResponce &)), &infDialog, SLOT(finishServerProperty(const EventsInfo::ServerPropertyInfoResponce &))));
                VERIFY(connect(server.get(), SIGNAL(startedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoRequest &)), &infDialog, SLOT(startServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoResponce &)), &infDialog, SLOT(finishServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoResponce &))));
                VERIFY(connect(&infDialog, SIGNAL(changedProperty(const PropertyType&)), server.get(), SLOT(changeProperty(const PropertyType&))));
                VERIFY(connect(&infDialog, SIGNAL(showed()), server.get(), SLOT(serverProperty())));
                infDialog.exec();
            }
        }
    }

    void ExplorerTreeView::openHistoryServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerSPtr server = node->server();
                ServerHistoryDialog histDialog(server->name() + " history", server->connectionType(), this);
                VERIFY(connect(server.get(), SIGNAL(startedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &)), &histDialog, SLOT(startLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &))));
                VERIFY(connect(server.get(), SIGNAL(finishedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &)), &histDialog, SLOT(finishLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &))));
                VERIFY(connect(&histDialog, SIGNAL(showed()), server.get(), SLOT(requestHistoryInfo())));
                histDialog.exec();
            }
        }
    }

    void ExplorerTreeView::closeConnection()
    {
        QModelIndex sel = selectedIndex();
        if(sel.isValid()){
            ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
            if(node){
                IServerSPtr server = node->server();
                removeServer(server);
            }
        }        
    }
}
