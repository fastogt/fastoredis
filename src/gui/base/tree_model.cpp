#include "gui/base/tree_model.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    TreeModel::TreeModel(QObject *parent)
        : QAbstractItemModel(parent), _root(new TreeItem(NULL))
    {

    }

    int TreeModel::rowCount(const QModelIndex &parent) const
    {
        const TreeItem *parentItem=NULL;
        if (parent.isValid()){
            parentItem = common::utils_qt::item<TreeItem*>(parent);
        }
        else{
            parentItem = _root.get();
        }

        assert(parentItem);
        return parentItem ? parentItem->childrenCount() : 0;
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
    {
        if (hasIndex(row, column, parent)) {
            const TreeItem * parentItem = NULL;
            if (!parent.isValid()) {
                parentItem = _root.get();
            } else {
                parentItem = common::utils_qt::item<TreeItem*>(parent);
            }

            TreeItem *childItem = parentItem->child(row);
            if (childItem) {
                return createIndex(row, column, childItem);
            }
            return QModelIndex();
        }
        return QModelIndex();
    }

    QModelIndex TreeModel::parent(const QModelIndex& index) const
    {
        if (!index.isValid()) {
            return QModelIndex();
        }

        TreeItem * childItem = common::utils_qt::item<TreeItem*>(index);
        if(childItem){
            TreeItem* parentItem = childItem->parent();
            if (parentItem && parentItem != _root.get()) {
                TreeItem * grandParent = parentItem->parent();
                int row = grandParent->indexOf(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }

        return QModelIndex();
    }

    TreeModel::~TreeModel()
    {

    }
}
