#include "gui/explorer/explorer_tree_model.h"

#include "gui/gui_factory.h"
#include "common/qt/utils_qt.h"

namespace fastoredis
{
    IExplorerTreeItem::IExplorerTreeItem(TreeItem *parent)
        : TreeItem(parent)
    {

    }

    IExplorerTreeItem::~IExplorerTreeItem()
    {

    }

    ExplorerServerItem::ExplorerServerItem(IServerPtr server, TreeItem *parent)
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

    IServerPtr ExplorerServerItem::server() const
    {
        return server_;
    }

    QString ExplorerServerItem::name() const
    {
        return server_->name();
    }

    ExplorerDatabaseItem::ExplorerDatabaseItem(const DataBaseInfo &db, ExplorerServerItem *parent)
        : IExplorerTreeItem(parent), db_(db)
    {

    }

    ExplorerDatabaseItem::~ExplorerDatabaseItem()
    {

    }

    void ExplorerDatabaseItem::loadContent()
    {
        server()->loadDatabaseContent(db_);
    }

    DataBaseInfo ExplorerDatabaseItem::db() const
    {
        return db_;
    }

    IServerPtr ExplorerDatabaseItem::server() const
    {
        return dynamic_cast<ExplorerServerItem*>(parent_)->server();
    }

    ExplorerDatabaseItem::eType ExplorerDatabaseItem::type() const
    {
        return Database;
    }

    QString ExplorerDatabaseItem::name() const
    {
        return common::convertFromString<QString>(db_.name_);
    }

    ExplorerServerItem *ExplorerDatabaseItem::parent() const
    {
        return dynamic_cast<ExplorerServerItem*>(IExplorerTreeItem::parent());
    }

    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : TreeModel(parent)
    {
    }

    QVariant ExplorerTreeModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()){
            return QVariant();
        }

        IExplorerTreeItem *node = common::utils_qt::item<IExplorerTreeItem*>(index);

        if (!node){
            return QVariant();
        }

        int col = index.column();

        if(role == Qt::DecorationRole && col == ExplorerServerItem::eName ){
            IExplorerTreeItem::eType t = node->type();
            if(t == IExplorerTreeItem::Server){
                return GuiFactory::instance().getIcon(node->server()->connectionType());
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

        return QVariant();
    }

    QVariant ExplorerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == ExplorerServerItem::eName) {
                return tr("Name");
            }
        }

        return TreeModel::headerData(section,orientation,role);
    }

    int ExplorerTreeModel::columnCount(const QModelIndex &parent) const
    {
        return ExplorerServerItem::eCountColumns;
    }

    Qt::ItemFlags ExplorerTreeModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags result = 0;
        if (index.isValid()) {
            result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        return result;
    }

    void ExplorerTreeModel::addServer(IServerPtr server)
    {
        ExplorerServerItem *serv = findServerItem(server.get());
        if(!serv){
            TreeItem *parent = dynamic_cast<TreeItem*>(_root.get());
            DCHECK(parent);
            int child_count = parent->childrenCount();
            beginInsertRows(QModelIndex(), child_count, child_count);
                ExplorerServerItem *item = new ExplorerServerItem(server, parent);
                parent->addChildren(item);
            endInsertRows();
        }
    }

    void ExplorerTreeModel::removeServer(IServerPtr server)
    {
        TreeItem *par = dynamic_cast<TreeItem*>(_root.get());
        DCHECK(par);
        ExplorerServerItem *serverItem = findServerItem(server.get());
        int row = par->indexOf(serverItem);
        beginRemoveRows(QModelIndex(), row, row);
            par->removeChildren(serverItem);
            delete serverItem;
        endRemoveRows();
    }

    void ExplorerTreeModel::addDatabase(IServer * server, const DataBaseInfo &db)
    {
        ExplorerServerItem *parent = findServerItem(server);
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

    ExplorerServerItem *ExplorerTreeModel::findServerItem(IServer *server) const
    {
        TreeItem *parent = dynamic_cast<TreeItem*>(_root.get());
        DCHECK(parent);
        for(int i = 0; i < parent->childrenCount() ; ++i){
            ExplorerServerItem *item = dynamic_cast<ExplorerServerItem*>(parent->child(i));
            DCHECK(item);
            if(item->server().get() == server){
                return item;
            }
        }
        return NULL;
    }

    ExplorerDatabaseItem *ExplorerTreeModel::findDatabaseItem(ExplorerServerItem *server, const DataBaseInfo &db) const
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
