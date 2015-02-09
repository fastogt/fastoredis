#include "gui/explorer/explorer_tree_model.h"

#include "translations/global.h"

#include "gui/gui_factory.h"

#include "common/qt/utils_qt.h"
#include "common/qt/convert_string.h"

namespace fastoredis
{
    IExplorerTreeItem::IExplorerTreeItem(TreeItem* parent)
        : TreeItem(parent)
    {

    }

    IExplorerTreeItem::~IExplorerTreeItem()
    {

    }

    ExplorerServerItem::ExplorerServerItem(IServerSPtr server, TreeItem* parent)
        : IExplorerTreeItem(parent), server_(server)
    {

    }

    ExplorerServerItem::~ExplorerServerItem()
    {

    }

    ExplorerServerItem::eType ExplorerServerItem::type() const
    {
        return Server;
    }

    IServerSPtr ExplorerServerItem::server() const
    {
        return server_;
    }

    void ExplorerServerItem::loadDatabases()
    {
         return server_->loadDatabases();
    }

    QString ExplorerServerItem::name() const
    {
        return server_->name();
    }

    ExplorerDatabaseItem::ExplorerDatabaseItem(DataBaseInfoSPtr db, ExplorerServerItem* parent)
        : IExplorerTreeItem(parent)
    {
        DCHECK(db);
        if(!db){
            return;
        }

        inf_.reset(db->clone());
    }

    ExplorerDatabaseItem::~ExplorerDatabaseItem()
    {

    }

    void ExplorerDatabaseItem::loadContent()
    {
        IDatabaseSPtr dbs = db();
        if(dbs){
            dbs->loadContent();
        }
    }

    IDatabaseSPtr ExplorerDatabaseItem::db() const
    {
        IServerSPtr serv = server();
        DCHECK(serv);
        if(!serv){
            return IDatabaseSPtr();
        }

        IDatabaseSPtr db = serv->findDatabaseByName(inf_->name());
        DCHECK(db);
        return db;
    }

    void ExplorerDatabaseItem::setDefault()
    {
        IDatabaseSPtr dbs = db();
        if(dbs){
            dbs->setDefault();
        }
    }

    void ExplorerDatabaseItem::removeKey(const std::string& key)
    {
        IDatabaseSPtr dbs = db();
        if(dbs){
            dbs->removeKey(key);
        }
    }

    void ExplorerDatabaseItem::loadValue(const QString& key)
    {
        IDatabaseSPtr dbs = db();
        if(dbs){
            dbs->loadValue(common::convertToString(key));
        }
    }

    DataBaseInfoSPtr ExplorerDatabaseItem::info() const
    {
        return inf_;
    }

    bool ExplorerDatabaseItem::isDefault() const
    {
        return inf_->isDefault();
    }

    IServerSPtr ExplorerDatabaseItem::server() const
    {
        ExplorerServerItem* serv = dynamic_cast<ExplorerServerItem*>(parent_);
        if(!serv){
            return IServerSPtr();
        }

        return serv->server();
    }

    ExplorerDatabaseItem::eType ExplorerDatabaseItem::type() const
    {
        return Database;
    }

    QString ExplorerDatabaseItem::name() const
    {
        return common::convertFromString<QString>(inf_->name());
    }

    ExplorerServerItem *ExplorerDatabaseItem::parent() const
    {
        return dynamic_cast<ExplorerServerItem*>(IExplorerTreeItem::parent());
    }

    ExplorerKeyItem::ExplorerKeyItem(const std::string& name, ExplorerDatabaseItem* parent)
        : IExplorerTreeItem(parent), name_(name)
    {

    }

    ExplorerKeyItem::~ExplorerKeyItem()
    {

    }

    QString ExplorerKeyItem::name() const
    {
        return common::convertFromString<QString>(name_);
    }

    std::string ExplorerKeyItem::sname() const
    {
        return name_;
    }

    IServerSPtr ExplorerKeyItem::server() const
    {
        ExplorerDatabaseItem* db = dynamic_cast<ExplorerDatabaseItem*>(parent_);
        if(!db){
            return IServerSPtr();
        }

        return db->server();
    }

    IExplorerTreeItem::eType ExplorerKeyItem::type() const
    {
        return Key;
    }

    void ExplorerKeyItem::remove()
    {
        ExplorerDatabaseItem* par = parent();
        if(par){
            par->removeKey(name_);
        }
    }

    ExplorerDatabaseItem* ExplorerKeyItem::parent() const
    {
        return dynamic_cast<ExplorerDatabaseItem*>(parent_);
    }

    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : TreeModel(parent)
    {
    }

    QVariant ExplorerTreeModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid()){
            return QVariant();
        }

        IExplorerTreeItem *node = common::utils_qt::item<IExplorerTreeItem*>(index);

        if (!node){
            return QVariant();
        }

        int col = index.column();

        IExplorerTreeItem::eType t = node->type();

        if(role == Qt::DecorationRole && col == ExplorerServerItem::eName ){            
            if(t == IExplorerTreeItem::Server){
                return GuiFactory::instance().icon(node->server()->type());
            }
            else if(t == IExplorerTreeItem::Key){
                return GuiFactory::instance().keyIcon();
            }
            else if(t == IExplorerTreeItem::Database){
                return GuiFactory::instance().databaseIcon();
            }
            else{
                NOTREACHED();
            }
        }

        if (role == Qt::DisplayRole) {
            if (col == IExplorerTreeItem::eName) {
                if(t == IExplorerTreeItem::Key){
                    return node->name();
                }
                else{
                    return QString("%1 (%2)").arg(node->name()).arg(node->childrenCount());
                }
            }
        }

        if(role == Qt::ForegroundRole){
            if(t == IExplorerTreeItem::Database){
                ExplorerDatabaseItem* db = dynamic_cast<ExplorerDatabaseItem*>(node);
                if(db && db->isDefault()){
                    return QVariant( QColor( Qt::red ) );
                }
            }
        }

        return QVariant();
    }

    QVariant ExplorerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == ExplorerServerItem::eName) {
                return translations::trName;
            }
        }

        return TreeModel::headerData(section,orientation,role);
    }

    int ExplorerTreeModel::columnCount(const QModelIndex& parent) const
    {
        return ExplorerServerItem::eCountColumns;
    }

    Qt::ItemFlags ExplorerTreeModel::flags(const QModelIndex& index) const
    {
        Qt::ItemFlags result = 0;
        if (index.isValid()) {
            result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        return result;
    }

    void ExplorerTreeModel::addServer(IServerSPtr server)
    {
        if(!server){
            return;
        }

        ExplorerServerItem *serv = findServerItem(server.get());
        if(!serv){
            TreeItem *parent = dynamic_cast<TreeItem*>(root_);
            DCHECK(parent);
            if(!parent){
                return;
            }

            ExplorerServerItem *item = new ExplorerServerItem(server, parent);
            insertItem(QModelIndex(), item);
        }
    }

    void ExplorerTreeModel::removeServer(IServerSPtr server)
    {
        TreeItem *par = dynamic_cast<TreeItem*>(root_);
        DCHECK(par);
        if(!par){
            return;
        }

        ExplorerServerItem *serverItem = findServerItem(server.get());
        removeItem(QModelIndex(), serverItem);
    }

    void ExplorerTreeModel::addDatabase(IServer* server, DataBaseInfoSPtr db)
    {
        ExplorerServerItem *parent = findServerItem(server);
        DCHECK(parent);
        if(!parent){
            return;
        }

        ExplorerDatabaseItem *dbs = findDatabaseItem(parent, db);
        if(!dbs){
            QModelIndex index = createIndex(0,0,parent);
            ExplorerDatabaseItem *item = new ExplorerDatabaseItem(db, parent);
            insertItem(index, item);
        }
    }

    void ExplorerTreeModel::removeDatabase(IServer* server, DataBaseInfoSPtr db)
    {
        ExplorerServerItem *parent = findServerItem(server);
        DCHECK(parent);
        if(!parent){
            return;
        }

        ExplorerDatabaseItem *dbs = findDatabaseItem(parent, db);
        if(!dbs){
            removeItem(createIndex(0,0,parent), dbs);
        }
    }

    void ExplorerTreeModel::setDefaultDb(IServer* server, DataBaseInfoSPtr db)
    {
        ExplorerServerItem *parent = findServerItem(server);
        DCHECK(parent);
        if(!parent){
            return;
        }

        int child_count = parent->childrenCount();
        for(int i = 0; i < child_count ; ++i){
            ExplorerDatabaseItem *item = dynamic_cast<ExplorerDatabaseItem*>(parent->child(i));
            DCHECK(item);
            if(!item){
                continue;
            }

            DataBaseInfoSPtr info = item->info();
            if(info->isDefault()){
                if(info->name() != db->name()){
                    info->setIsDefault(false);
                    updateItem(createIndex(i,0,parent), createIndex(i,0,parent));
                }
            }
            else{
                if(info->name() == db->name()){
                    info->setIsDefault(true);
                    updateItem(createIndex(i,0,parent), createIndex(i,0,parent));
                }
            }
        }
    }

    void ExplorerTreeModel::addKey(IServer* server, DataBaseInfoSPtr db, const std::string& key)
    {
        ExplorerServerItem *parent = findServerItem(server);
        DCHECK(parent);
        if(!parent){
            return;
        }

        ExplorerDatabaseItem *dbs = findDatabaseItem(parent, db);
        DCHECK(dbs);
        if(!dbs){
            return;
        }

        ExplorerKeyItem *keyit = findKeyItem(dbs, key);
        if(!keyit){
            QModelIndex parentdb = createIndex(parent->indexOf(dbs), 0, dbs);
            ExplorerKeyItem *item = new ExplorerKeyItem(key, dbs);
            insertItem(parentdb, item);
        }
    }

    void ExplorerTreeModel::removeKey(IServer* server, DataBaseInfoSPtr db, const std::string& key)
    {
        ExplorerServerItem *parent = findServerItem(server);
        DCHECK(parent);
        if(!parent){
            return;
        }

        ExplorerDatabaseItem *dbs = findDatabaseItem(parent, db);
        DCHECK(dbs);
        if(!dbs){
            return;
        }

        ExplorerKeyItem *keyit = findKeyItem(dbs, key);
        if(keyit){
            //QModelIndex parentdb = createIndex(parent->indexOf(dbs), 0, dbs);
            //removeItem(parentdb, keyit);
            dbs->removeChildren(keyit);
        }
    }

    ExplorerServerItem *ExplorerTreeModel::findServerItem(IServer* server) const
    {
        TreeItem *parent = dynamic_cast<TreeItem*>(root_);
        DCHECK(parent);
        if(!parent){
            return NULL;
        }
        for(int i = 0; i < parent->childrenCount() ; ++i){
            ExplorerServerItem *item = dynamic_cast<ExplorerServerItem*>(parent->child(i));
            DCHECK(item);
            if(item->server().get() == server){
                return item;
            }
        }
        return NULL;
    }

    ExplorerDatabaseItem *ExplorerTreeModel::findDatabaseItem(ExplorerServerItem* server, DataBaseInfoSPtr db) const
    {
        if(server){
            for(int i = 0; i < server->childrenCount() ; ++i){
                ExplorerDatabaseItem *item = dynamic_cast<ExplorerDatabaseItem*>(server->child(i));
                DCHECK(item);
                if(!item){
                    continue;
                }

                IDatabaseSPtr inf = item->db();
                if(inf && inf->name() == db->name()){
                    return item;
                }
            }
        }
        return NULL;
    }

    ExplorerKeyItem *ExplorerTreeModel::findKeyItem(ExplorerDatabaseItem* db, const std::string& key) const
    {
        if(db){
            for(int i = 0; i < db->childrenCount() ; ++i){
                ExplorerKeyItem *item = dynamic_cast<ExplorerKeyItem*>(db->child(i));
                DCHECK(item);
                if(!item){
                    continue;
                }

                if(item->sname() == key){
                    return item;
                }
            }
        }
        return NULL;
    }

    ExplorerTreeModel::~ExplorerTreeModel()
    {

    }
}
