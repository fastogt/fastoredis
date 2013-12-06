#include "gui/FastoTreeModel.h"

#include "gui/FastoTreeItem.h"
#include "gui/GuiFactory.h"
#include "common/qt_helper/utils_qt.h"
#include "common/qt_helper/converter_patterns.h"

namespace fastoredis
{
    FastoTreeModel::FastoTreeModel(QObject *parent)
        : base_class(parent), _root(NULL)
    {

    }

    void FastoTreeModel::setRoot(FastoTreeItem *root)
    {
        beginResetModel();
        _root.reset(root);
        endResetModel();
    }

    QVariant FastoTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        FastoTreeItem *node = common::utils_qt::item<FastoTreeItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if(role == Qt::DecorationRole && col == FastoTreeItem::eKey ){
            return GuiFactory::instance().getIcon(node->type());
        }

        if(role == Qt::TextColorRole && col == FastoTreeItem::eType){
            return QColor(Qt::gray);
        }

        if (role == Qt::DisplayRole) {
            if (col == FastoTreeItem::eKey) {
                result = node->key();
            }
            else if (col == FastoTreeItem::eValue) {
                result = node->value();
            }
            else if (col == FastoTreeItem::eType) {
                result = common::utils_qt::toQString(detail::toStdString(node->type()));
            }
        }

        return result;
    }

    int FastoTreeModel::rowCount(const QModelIndex &parent) const
    {
        const FastoTreeItem *parentItem=NULL;
        if (parent.isValid())
            parentItem = common::utils_qt::item<FastoTreeItem*>(parent);
        else
            parentItem = _root.get();

        return parentItem ? parentItem->childrenCount() : 0;
    }

    QVariant FastoTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == FastoTreeItem::eKey) {
                return "Key";
            }
            else if (section == FastoTreeItem::eValue) {
                return "Value";
            }
            else {
                return "Type";
            }
        }

        return base_class::headerData(section,orientation,role);
    }

    int FastoTreeModel::columnCount(const QModelIndex &parent) const
    {
        return FastoTreeItem::eCountColumns;
    }

    QModelIndex FastoTreeModel::index(int row, int column, const QModelIndex &parent) const
    {
        QModelIndex index;
        if (hasIndex(row, column, parent)) {
            const FastoTreeItem * parentItem = NULL;
            if (!parent.isValid()) {
                parentItem = _root.get();
            } else {
                parentItem = common::utils_qt::item<FastoTreeItem*>(parent);
            }

            FastoTreeItem *childItem = parentItem->child(row);
            if (childItem) {
                index = createIndex(row, column, childItem);
            }
        }
        return index;
    }

    QModelIndex FastoTreeModel::parent(const QModelIndex& index) const
    {
        QModelIndex result;
        if (index.isValid()) {
            FastoTreeItem * childItem = common::utils_qt::item<FastoTreeItem*>(index);
            if(childItem){
                FastoTreeItem * parentItem = childItem->parent();
                if (parentItem && parentItem!=_root.get()) {
                    FastoTreeItem * grandParent = parentItem->parent();
                    int row = grandParent->indexOf(parentItem);
                    result= createIndex(row, 0, parentItem);
                }
            }
        }
        return result;
    }

    Qt::ItemFlags FastoTreeModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags result = 0;
        if (index.isValid()) {
            result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        return result;
    }

    FastoTreeModel::~FastoTreeModel()
    {

    }
}
