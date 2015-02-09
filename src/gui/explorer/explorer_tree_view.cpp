#include "gui/explorer/explorer_tree_view.h"

#include <QMenu>
#include <QMessageBox>
#include <QHeaderView>
#include <QAction>
#include <QFileDialog>

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

        infoServerAction_ = new QAction(this);
        VERIFY(connect(infoServerAction_, SIGNAL(triggered()), SLOT(openInfoServerDialog())));

        propertyServerAction_ = new QAction(this);
        VERIFY(connect(propertyServerAction_, SIGNAL(triggered()), SLOT(openPropertyServerDialog())));

        historyServerAction_ = new QAction(this);
        VERIFY(connect(historyServerAction_, SIGNAL(triggered()), SLOT(openHistoryServerDialog())));

        closeAction_ = new QAction(this);
        VERIFY(connect(closeAction_, SIGNAL(triggered()), SLOT(closeConnection())));

        importAction_ = new QAction(this);
        VERIFY(connect(importAction_, SIGNAL(triggered()), SLOT(importServer())));

        backupAction_ = new QAction(this);
        VERIFY(connect(backupAction_, SIGNAL(triggered()), SLOT(backupServer())));

        shutdownAction_ = new QAction(this);
        VERIFY(connect(shutdownAction_, SIGNAL(triggered()), SLOT(shutdownServer())));

        loadContentAction_ = new QAction(this);
        VERIFY(connect(loadContentAction_, SIGNAL(triggered()), SLOT(loadContentDb())));

        setDefaultDbAction_ = new QAction(this);
        VERIFY(connect(setDefaultDbAction_, SIGNAL(triggered()), SLOT(setDefaultDb())));

        getValueAction_ = new QAction(this);
        VERIFY(connect(getValueAction_, SIGNAL(triggered()), SLOT(getValue())));

        deleteKeyAction_ = new QAction(this);
        VERIFY(connect(deleteKeyAction_, SIGNAL(triggered()), SLOT(deleteKey())));

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

                bool isLocal = server->isLocalHost();

                importAction_->setEnabled(!isCon && isLocal);
                menu.addAction(importAction_);                
                backupAction_->setEnabled(isCon && isLocal);
                menu.addAction(backupAction_);
                shutdownAction_->setEnabled(isCon);
                menu.addAction(shutdownAction_);

                menu.exec(menuPoint);
            }
            else if(node->type() == IExplorerTreeItem::Database){
                ExplorerDatabaseItem *db = dynamic_cast<ExplorerDatabaseItem*>(node);
                QMenu menu(this);
                menu.addAction(loadContentAction_);
                bool isDefault = db && db->isDefault();
                loadContentAction_->setEnabled(isDefault);

                menu.addAction(setDefaultDbAction_);
                setDefaultDbAction_->setEnabled(!isDefault);
                menu.exec(menuPoint);
            }
            else if(node->type() == IExplorerTreeItem::Key){
                QMenu menu(this);
                menu.addAction(getValueAction_);
                menu.addAction(deleteKeyAction_);
                menu.exec(menuPoint);
            }
        }
    }

    void ExplorerTreeView::connectDisconnectToServer()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();
        if(!server){
            return;
        }

        if(server->isConnected()){
            server->disconnect();
        }
        else{
            server->connect();
        }
    }

    void ExplorerTreeView::openConsole()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(node){
            emit openedConsole(node->server());
        }
    }

    void ExplorerTreeView::loadDatabases()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(node){
            node->loadDatabases();
        }
    }

    void ExplorerTreeView::loadContentDb()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerDatabaseItem *node = common::utils_qt::item<ExplorerDatabaseItem*>(sel);
        if(node){
            node->loadContent();
        }
    }

    void ExplorerTreeView::setDefaultDb()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerDatabaseItem *node = common::utils_qt::item<ExplorerDatabaseItem*>(sel);
        if(node){
            node->setDefault();
        }
    }

    void ExplorerTreeView::getValue()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerKeyItem *node = common::utils_qt::item<ExplorerKeyItem*>(sel);
        if(node){
            emit executeText(node->server(), QString("GET %1").arg(node->name()));
        }
    }

    void ExplorerTreeView::deleteKey()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerKeyItem *node = common::utils_qt::item<ExplorerKeyItem*>(sel);
        if(node){
            node->remove();
        }
    }

    void ExplorerTreeView::addServer(IServerSPtr server)
    {
        DCHECK(server);
        if(!server){
            return;
        }

        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        VERIFY(connect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
        VERIFY(connect(server.get(), SIGNAL(startedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest &)), this, SLOT(startSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce &)), this, SLOT(finishSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce &))));
        VERIFY(connect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabaseContentRequest &)), this, SLOT(startLoadDatabaseContent(const EventsInfo::LoadDatabaseContentRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce &)), this, SLOT(finishLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce &))));
        VERIFY(connect(server.get(), SIGNAL(startedExecuteCommand(const EventsInfo::CommandRequest &)), this, SLOT(startExecuteCommand(const EventsInfo::CommandRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedExecuteCommand(const EventsInfo::CommandResponce &)), this, SLOT(finishExecuteCommand(const EventsInfo::CommandResponce &))));

        mod->addServer(server);
    }

    void ExplorerTreeView::removeServer(IServerSPtr server)
    {
        DCHECK(server);
        if(!server){
            return;
        }

        ExplorerTreeModel *mod = static_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        VERIFY(disconnect(server.get(), SIGNAL(startedLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &)), this, SLOT(startLoadDatabases(const EventsInfo::LoadDatabasesInfoRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &)), this, SLOT(finishLoadDatabases(const EventsInfo::LoadDatabasesInfoResponce &))));
        VERIFY(disconnect(server.get(), SIGNAL(startedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest &)), this, SLOT(startSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce &)), this, SLOT(finishSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce &))));
        VERIFY(disconnect(server.get(), SIGNAL(startedLoadDataBaseContent(const EventsInfo::LoadDatabaseContentRequest &)), this, SLOT(startLoadDatabaseContent(const EventsInfo::LoadDatabaseContentRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce &)), this, SLOT(finishLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce &))));
        VERIFY(disconnect(server.get(), SIGNAL(startedExecuteCommand(const EventsInfo::CommandRequest &)), this, SLOT(startExecuteCommand(const EventsInfo::CommandRequest &))));
        VERIFY(disconnect(server.get(), SIGNAL(finishedExecuteCommand(const EventsInfo::CommandResponce &)), this, SLOT(finishExecuteCommand(const EventsInfo::CommandResponce &))));

        mod->removeServer(server);
        emit closeServer(server);
    }

    QModelIndex ExplorerTreeView::selectedIndex() const
    {
        QModelIndexList indexses = selectionModel()->selectedRows();

        if (indexses.count() != 1){
            return QModelIndex();
        }

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
        infoServerAction_->setText(trInfo);
        propertyServerAction_->setText(trProperty);
        historyServerAction_->setText(trHistory);
        closeAction_->setText(trClose);
        backupAction_->setText(trBackup);
        importAction_->setText(trImport);
        shutdownAction_->setText(trShutdown);

        loadContentAction_->setText(trLoadContOfDataBases);
        setDefaultDbAction_->setText(trSetDefault);
        getValueAction_->setText(trValue);
        deleteKeyAction_->setText(trDelete);
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
        if(!serv){
            return;
        }

        ExplorerTreeModel *mod = qobject_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        EventsInfo::LoadDatabasesInfoResponce::database_info_cont_type dbs = res.databases_;

        for(int i = 0; i < dbs.size(); ++i){
            DataBaseInfoSPtr db = dbs[i];
            mod->addDatabase(serv, db);
        }
    }

    void ExplorerTreeView::startSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseRequest& req)
    {

    }

    void ExplorerTreeView::finishSetDefaultDatabase(const EventsInfo::SetDefaultDatabaseResponce& res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        IServer *serv = qobject_cast<IServer *>(sender());
        DCHECK(serv);
        if(!serv){
            return;
        }

        DataBaseInfoSPtr db = res.inf_;
        ExplorerTreeModel *mod = qobject_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        mod->setDefaultDb(serv, db);
    }

    void ExplorerTreeView::startLoadDatabaseContent(const EventsInfo::LoadDatabaseContentRequest &req)
    {

    }

    void ExplorerTreeView::finishLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce &res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        IServer *serv = qobject_cast<IServer *>(sender());
        DCHECK(serv);
        if(!serv){
            return;
        }

        ExplorerTreeModel *mod = qobject_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        EventsInfo::LoadDatabaseContentResponce::keys_cont_type keys = res.keys_;

        for(int i = 0; i < keys.size(); ++i){
            std::string key = keys[i];
            mod->addKey(serv, res.inf_, key);
        }
    }

    void ExplorerTreeView::startExecuteCommand(const EventsInfo::CommandRequest &req)
    {

    }

    void ExplorerTreeView::finishExecuteCommand(const EventsInfo::CommandResponce &res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        IServer* serv = qobject_cast<IServer *>(sender());
        DCHECK(serv);
        if(!serv){
            return;
        }

        ExplorerTreeModel* mod = qobject_cast<ExplorerTreeModel*>(model());
        DCHECK(mod);
        if(!mod){
            return;
        }

        CommandKey key = res.cmd_;
        if(key.type() == CommandKey::DELETE){
            mod->removeKey(serv, res.inf_, key.key());
        }
    }

    QModelIndexList ExplorerTreeView::selectedIndexes() const
    {
        return selectionModel()->selectedRows();
    }

    void ExplorerTreeView::openInfoServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();
        if(!server){
            return;
        }

        InfoServerDialog infDialog(QString("%1 info").arg(server->name()), server->type(), this);
        VERIFY(connect(server.get(), SIGNAL(startedLoadServerInfo(const EventsInfo::ServerInfoRequest &)), &infDialog, SLOT(startServerInfo(const EventsInfo::ServerInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadServerInfo(const EventsInfo::ServerInfoResponce &)), &infDialog, SLOT(finishServerInfo(const EventsInfo::ServerInfoResponce &))));
        VERIFY(connect(&infDialog, SIGNAL(showed()), server.get(), SLOT(serverInfo())));
        infDialog.exec();
    }

    void ExplorerTreeView::openPropertyServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();
        if(!server){
            return;
        }

        PropertyServerDialog infDialog(QString("%1 properties").arg(server->name()), server->type(), this);
        VERIFY(connect(server.get(), SIGNAL(startedLoadServerProperty(const EventsInfo::ServerPropertyInfoRequest &)), &infDialog, SLOT(startServerProperty(const EventsInfo::ServerPropertyInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadServerProperty(const EventsInfo::ServerPropertyInfoResponce &)), &infDialog, SLOT(finishServerProperty(const EventsInfo::ServerPropertyInfoResponce &))));
        VERIFY(connect(server.get(), SIGNAL(startedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoRequest &)), &infDialog, SLOT(startServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedChangeServerProperty(const EventsInfo::ChangeServerPropertyInfoResponce &)), &infDialog, SLOT(finishServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoResponce &))));
        VERIFY(connect(&infDialog, SIGNAL(changedProperty(const PropertyType&)), server.get(), SLOT(changeProperty(const PropertyType&))));
        VERIFY(connect(&infDialog, SIGNAL(showed()), server.get(), SLOT(serverProperty())));
        infDialog.exec();
    }

    void ExplorerTreeView::openHistoryServerDialog()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();
        if(!server){
            return;
        }

        ServerHistoryDialog histDialog(QString("%1 history").arg(server->name()), server->type(), this);
        VERIFY(connect(server.get(), SIGNAL(startedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &)), &histDialog, SLOT(startLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &))));
        VERIFY(connect(server.get(), SIGNAL(finishedLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &)), &histDialog, SLOT(finishLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &))));
        VERIFY(connect(server.get(), SIGNAL(serverInfoSnapShoot(ServerInfoSnapShoot )), &histDialog, SLOT(snapShotAdd(ServerInfoSnapShoot ))));
        VERIFY(connect(&histDialog, SIGNAL(showed()), server.get(), SLOT(requestHistoryInfo())));
        histDialog.exec();
    }

    void ExplorerTreeView::closeConnection()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(node){
            IServerSPtr server = node->server();
            if(server){
                removeServer(server);
            }
        }
    }

    void ExplorerTreeView::backupServer()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();

        using namespace translations;
        QString filepath = QFileDialog::getOpenFileName(this, trBackup, QString(), trfilterForRdb);
        if (!filepath.isEmpty() && server) {
            server->backupToPath(filepath);
        }
    }

    void ExplorerTreeView::importServer()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();

        using namespace translations;
        QString filepath = QFileDialog::getOpenFileName(this, trImport, QString(), trfilterForRdb);
        if (filepath.isEmpty() && server) {
            server->exportFromPath(filepath);
        }
    }

    void ExplorerTreeView::shutdownServer()
    {
        QModelIndex sel = selectedIndex();
        if(!sel.isValid()){
            return;
        }

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(sel);
        if(!node){
            return;
        }

        IServerSPtr server = node->server();
        if(server && server->isConnected()){
            // Ask user
            int answer = QMessageBox::question(this, "Shutdown", QString("Really shutdown \"%1\" server?").arg(server->name()), QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);

            if (answer != QMessageBox::Yes){
                return;
            }

            server->shutDown();
        }
    }
}
