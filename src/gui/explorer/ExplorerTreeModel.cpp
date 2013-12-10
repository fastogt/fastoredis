#include "gui/explorer/ExplorerTreeModel.h"

#include "gui/GuiFactory.h"
#include "common/qt_helper/utils_qt.h"
#include "common/qt_helper/converter_patterns.h"

namespace
{
    using namespace fastoredis;

    struct ExplorerTreeItem
            : public TreeItem
    {
        typedef TreeItem base_class;
        enum eColumn
        {
            eName = 0,
            eSize,
            eCountColumns
        };
        ExplorerTreeItem(ExplorerTreeItem *parent)
            : base_class(parent)
        {

        }
        QString _name;
        int _size;
    };
}

namespace fastoredis
{
    ExplorerTreeModel::ExplorerTreeModel(QObject *parent)
        : base_class(parent)
    {

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
                result = node->_name;
            }
            else if (col == ExplorerTreeItem::eSize) {
                result = node->_size;
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

    ExplorerTreeModel::~ExplorerTreeModel()
    {

    }
}
