#include "gui/explorer/ExplorerTreeModel.h"

#include "gui/GuiFactory.h"
#include "common/qt_helper/utils_qt.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    ExplorerTreeServerItem::ExplorerTreeServerItem(const IServerPtr &server, ExplorerTreeServerItem *parent)
        : base_class(parent), server_(server)
    {

    }

    IServerPtr ExplorerTreeServerItem::server() const
    {
        return server_;
    }

    QString ExplorerTreeServerItem::name() const
    {
        return server_->name();
    }

    ExplorerDatabaseItem::ExplorerDatabaseItem(ExplorerTreeServerItem *par, const QString &name)
        : base_class(par->server(), par), name_(name)
    {

    }

    IServerPtr ExplorerDatabaseItem::server() const
    {
        return static_cast<ExplorerTreeServerItem*>(parent_)->server();
    }

    QString ExplorerDatabaseItem::name() const
    {
        return name_;
    }

    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : base_class(parent)
    {
        _root.reset(new ExplorerTreeServerItem(IServerPtr(),NULL));
    }

    QVariant ExplorerTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        ExplorerTreeServerItem *node = common::utils_qt::item<ExplorerTreeServerItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if(role == Qt::DecorationRole && col == ExplorerTreeServerItem::eName ){
            return GuiFactory::instance().getIcon(node->server()->connectionType());
        }

        if (role == Qt::DisplayRole) {
            if (col == ExplorerTreeServerItem::eName) {
                result = node->name();
            }
            else if (col == ExplorerTreeServerItem::eStatus) {
                result = node->server()->isConnected() ? "Connected" : "Not Connected";
            }
        }

        return result;
    }

    QVariant ExplorerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == ExplorerTreeServerItem::eName) {
                return "Name";
            }
            else{
                return "Status";
            }
        }

        return base_class::headerData(section,orientation,role);
    }

    int ExplorerTreeModel::columnCount(const QModelIndex &parent) const
    {
        return ExplorerTreeServerItem::eCountColumns;
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
        ExplorerTreeServerItem *parent = static_cast<ExplorerTreeServerItem*>(_root.get());
        int child_count = parent->childrenCount();
        beginInsertRows(QModelIndex(),child_count,child_count);
            ExplorerTreeServerItem *item = new ExplorerTreeServerItem(server, parent);
            parent->addChildren(item);
        endInsertRows();
    }

    void ExplorerTreeModel::addDatabase(IServer *server, const QString &name)
    {
        ExplorerTreeServerItem *parent = NULL;
        for(int i = 0; i < _root->childrenCount(); ++i ){
            ExplorerTreeServerItem *item = static_cast<ExplorerTreeServerItem*>(_root->child(i));
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
        /*ExplorerTreeServerItem *parent = static_cast<ExplorerTreeServerItem*>(_root.get());
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
