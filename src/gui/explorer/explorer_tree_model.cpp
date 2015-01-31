#include "gui/explorer/explorer_tree_model.h"

#include "translations/global.h"

#include "gui/gui_factory.h"
#include "common/qt/utils_qt.h"

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

    QString ExplorerServerItem::name() const
    {
        return server_->name();
    }

    ExplorerDatabaseItem::ExplorerDatabaseItem(IDatabaseSPtr db, ExplorerServerItem* parent)
        : IExplorerTreeItem(parent), db_(db)
    {
        DCHECK(db_);
    }

    ExplorerDatabaseItem::~ExplorerDatabaseItem()
    {

    }

    void ExplorerDatabaseItem::loadContent()
    {
        db_->loadContent();
    }

    void ExplorerDatabaseItem::setDefault()
    {
       db_->setDefault();
    }

    bool ExplorerDatabaseItem::isDefault() const
    {
        return db_->isDefault();
    }

    IDatabaseSPtr ExplorerDatabaseItem::db() const
    {
        return db_;
    }

    IServerSPtr ExplorerDatabaseItem::server() const
    {
        return dynamic_cast<ExplorerServerItem*>(parent_)->server();
    }

    ExplorerDatabaseItem::eType ExplorerDatabaseItem::type() const
    {
        return Database;
    }

    QString ExplorerDatabaseItem::name() const
    {
        return common::convertFromString<QString>(db_->name());
    }

    ExplorerServerItem *ExplorerDatabaseItem::parent() const
    {
        return dynamic_cast<ExplorerServerItem*>(IExplorerTreeItem::parent());
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
            else{
                return GuiFactory::instance().databaseIcon();
            }
        }

        if (role == Qt::DisplayRole) {
            if (col == IExplorerTreeItem::eName) {                
                return node->name();
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
        ExplorerServerItem *serv = findServerItem(server.get());
        if(!serv){
            TreeItem *parent = dynamic_cast<TreeItem*>(root_);
            DCHECK(parent);
            if(!parent){
                return;
            }
            int child_count = parent->childrenCount();
            beginInsertRows(QModelIndex(), child_count, child_count);
                ExplorerServerItem *item = new ExplorerServerItem(server, parent);
                parent->addChildren(item);
            endInsertRows();
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
        int row = par->indexOf(serverItem);
        beginRemoveRows(QModelIndex(), row, row);
            par->removeChildren(serverItem);
            delete serverItem;
        endRemoveRows();
    }

    void ExplorerTreeModel::addDatabase(IDatabaseSPtr db)
    {
        ExplorerServerItem *parent = findServerItem(db->server().get());
        DCHECK(parent);
        ExplorerDatabaseItem *dbs = findDatabaseItem(parent, db);
        if(!dbs){
            int child_count = parent->childrenCount();
            QModelIndex index = createIndex(0,0,parent);
            beginInsertRows(index,child_count,child_count);
                ExplorerDatabaseItem *item = new ExplorerDatabaseItem(db, parent);
                parent->addChildren(item);
            endInsertRows();
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

    ExplorerDatabaseItem *ExplorerTreeModel::findDatabaseItem(ExplorerServerItem* server, IDatabaseSPtr db) const
    {
        ExplorerDatabaseItem *result = NULL;
        if(server){
            for(int i = 0; i < server->childrenCount() ; ++i){
                ExplorerDatabaseItem *item = dynamic_cast<ExplorerDatabaseItem*>(server->child(i));
                DCHECK(item);
                if(item->db() == db){
                    result = item;
                    break;
                }
            }
        }
        return result;
    }

    ExplorerTreeModel::~ExplorerTreeModel()
    {

    }
}
