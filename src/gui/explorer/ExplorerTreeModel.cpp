#include "gui/explorer/ExplorerTreeModel.h"

#include "gui/GuiFactory.h"
#include "common/qt_helper/utils_qt.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : base_class(parent)
    {
        _root.reset(new ExplorerTreeItem(NULL));
    }

    QVariant ExplorerTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        ExplorerTreeItem *node = common::utils_qt::item<ExplorerTreeItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if (role == Qt::DisplayRole) {
            if (col == ExplorerTreeItem::eName) {
                result = node->server_->name();
            }
            else if (col == ExplorerTreeItem::eSize) {
                result = 0;
            }
        }

        return result;
    }

    QVariant ExplorerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == ExplorerTreeItem::eName) {
                return "Name";
            }
            else{
                return "Size";
            }
        }

        return base_class::headerData(section,orientation,role);
    }

    int ExplorerTreeModel::columnCount(const QModelIndex &parent) const
    {
        return ExplorerTreeItem::eCountColumns;
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
        ExplorerTreeItem *parent = static_cast<ExplorerTreeItem*>(_root.get());
        int child_count = parent->childrenCount();
        beginInsertRows(QModelIndex(),child_count,child_count);
            ExplorerTreeItem *item = new ExplorerTreeItem(parent);
            item->server_ = server;
            parent->addChildren(item);
        endInsertRows();
    }

    void ExplorerTreeModel::removeServer(const IServerPtr &server)
    {
        /*ExplorerTreeItem *parent = static_cast<ExplorerTreeItem*>(_root.get());
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
