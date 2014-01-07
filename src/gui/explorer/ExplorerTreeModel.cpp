#include "gui/explorer/ExplorerTreeModel.h"

#include "gui/GuiFactory.h"
#include "common/qt/utils_qt.h"
#include "common/qt/converter_patterns.h"
#include "core/IDataBase.h"

namespace fastoredis
{
    IExplorerTreeItem::IExplorerTreeItem(TreeItem *parent)
        : TreeItem(parent)
    {

    }

    ExplorerServerItem::ExplorerServerItem(const IServerPtr &server, TreeItem *parent)
        : IExplorerTreeItem(parent), server_(server)
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
        return common::utils_qt::toQString(db_.name_);
    }

    ExplorerServerItem *ExplorerDatabaseItem::parent() const
    {
        return dynamic_cast<ExplorerServerItem*>(IExplorerTreeItem::parent());
    }

    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : TreeModel(parent)
    {
        _root.reset(new TreeItem(NULL));
    }

    QVariant ExplorerTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        IExplorerTreeItem *node = common::utils_qt::item<IExplorerTreeItem*>(index);

        if (!node)
            return result;

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
                result = node->name();
            }
        }

        return result;
    }

    QVariant ExplorerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == ExplorerServerItem::eName) {
                return "Name";
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

    void ExplorerTreeModel::addServer(const IServerPtr &server)
    {
        ExplorerServerItem *parent = dynamic_cast<ExplorerServerItem*>(_root.get());
        Q_ASSERT(parent);
        int child_count = parent->childrenCount();
        beginInsertRows(QModelIndex(),child_count,child_count);
            ExplorerServerItem *item = new ExplorerServerItem(server, parent);
            parent->addChildren(item);
        endInsertRows();
    }

    void ExplorerTreeModel::removeServer(const IServerPtr &server)
    {
        ExplorerServerItem *parent = dynamic_cast<ExplorerServerItem*>(_root.get());
        DCHECK(parent);
        QModelIndex index = createIndex(0,0,parent);
        ExplorerServerItem *serverItem = findServerItem(server.get());
        int row = parent->indexOf(serverItem);
        beginRemoveRows(index, row, row);
            parent->removeChildren(serverItem);
        endRemoveRows();
    }

    void ExplorerTreeModel::addDatabase(IServer *server, const DataBaseInfo &database)
    {
        ExplorerServerItem *parent = findServerItem(server);
        int child_count = parent->childrenCount();
        QModelIndex index = createIndex(0,0,parent);
        beginInsertRows(index,child_count,child_count);
            ExplorerDatabaseItem *item = new ExplorerDatabaseItem(database, parent);
            parent->addChildren(item);
        endInsertRows();
    }

    ExplorerServerItem *ExplorerTreeModel::findServerItem(IServer *server) const
    {
        ExplorerServerItem *parent = dynamic_cast<ExplorerServerItem*>(_root.get());
        DCHECK(parent);
        ExplorerServerItem *result = NULL;
        for(int i = 0; i < parent->childrenCount() ; ++i){
            ExplorerServerItem *item = dynamic_cast<ExplorerServerItem*>(_root->child(i));
            DCHECK(item);
            if(item->server().get() == server){
                parent = item;
                break;
            }
        }
        DCHECK(result);
        return result;
    }

    ExplorerTreeModel::~ExplorerTreeModel()
    {

    }
}
