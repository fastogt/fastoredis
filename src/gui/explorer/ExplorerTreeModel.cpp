#include "gui/explorer/ExplorerTreeModel.h"

#include "gui/GuiFactory.h"
#include "common/qt_helper/utils_qt.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    ExplorerServerItem::ExplorerServerItem(const IServerPtr &server, ExplorerServerItem *parent)
        : base_class(parent), server_(server)
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

    ExplorerDatabaseItem::ExplorerDatabaseItem(ExplorerServerItem *par, const QString &name)
        : base_class(par->server(), par), name_(name)
    {

    }

    IServerPtr ExplorerDatabaseItem::server() const
    {
        return static_cast<ExplorerServerItem*>(parent_)->server();
    }

    ExplorerDatabaseItem::eType ExplorerDatabaseItem::type() const
    {
        return Database;
    }

    QString ExplorerDatabaseItem::name() const
    {
        return name_;
    }

    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : base_class(parent)
    {
        _root.reset(new ExplorerServerItem(IServerPtr(),NULL));
    }

    QVariant ExplorerTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        ExplorerServerItem *node = common::utils_qt::item<ExplorerServerItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if(role == Qt::DecorationRole && col == ExplorerServerItem::eName ){
            ExplorerServerItem::eType t = node->type();
            if(t == ExplorerServerItem::Server){
                return GuiFactory::instance().getIcon(node->server()->connectionType());
            }
            else{
                return GuiFactory::instance().databaseIcon();
            }
        }

        if (role == Qt::DisplayRole) {
            if (col == ExplorerServerItem::eName) {
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

        return base_class::headerData(section,orientation,role);
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
        ExplorerServerItem *parent = static_cast<ExplorerServerItem*>(_root.get());
        int child_count = parent->childrenCount();
        beginInsertRows(QModelIndex(),child_count,child_count);
            ExplorerServerItem *item = new ExplorerServerItem(server, parent);
            parent->addChildren(item);
        endInsertRows();
    }

    void ExplorerTreeModel::addDatabase(IServer *server, const QString &name)
    {
        ExplorerServerItem *parent = NULL;
        for(int i = 0; i < _root->childrenCount(); ++i ){
            ExplorerServerItem *item = static_cast<ExplorerServerItem*>(_root->child(i));
            if(item->server().get() == server){
                parent = item;
                break;
            }
        }

        int child_count = parent->childrenCount();
        QModelIndex index = createIndex(0,0,parent);
        beginInsertRows(index,child_count,child_count);
            ExplorerDatabaseItem *item = new ExplorerDatabaseItem(parent, name);
            parent->addChildren(item);
        endInsertRows();
    }

    void ExplorerTreeModel::removeServer(const IServerPtr &server)
    {
        /*ExplorerServerItem *parent = static_cast<ExplorerServerItem*>(_root.get());
        QModelIndex index = createIndex(0,0,parent);
        int row = parent->indexOf(server);
        beginRemoveRows(index, row, row);
        parent->removeChildren(server);
        endRemoveRows();*/
    }

    ExplorerTreeModel::~ExplorerTreeModel()
    {

    }
}
