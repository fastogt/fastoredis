#include "gui/base/TreeModel.h"

#include "common/qt_helper/utils_qt.h"

namespace fastoredis
{
    TreeModel::TreeModel(QObject *parent)
        : QAbstractItemModel(parent), _root(NULL)
    {

    }

    int TreeModel::rowCount(const QModelIndex &parent) const
    {
        const TreeItem *parentItem=NULL;
        if (parent.isValid())
            parentItem = common::utils_qt::item<TreeItem*>(parent);
        else
            parentItem = _root.get();

        return parentItem ? parentItem->childrenCount() : 0;
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
    {
        QModelIndex index;
        if (hasIndex(row, column, parent)) {
            const TreeItem * parentItem = NULL;
            if (!parent.isValid()) {
                parentItem = _root.get();
            } else {
                parentItem = common::utils_qt::item<TreeItem*>(parent);
            }

            TreeItem *childItem = parentItem->child(row);
            if (childItem) {
                index = createIndex(row, column, childItem);
            }
        }
        return index;
    }

    QModelIndex TreeModel::parent(const QModelIndex& index) const
    {
        QModelIndex result;
        if (index.isValid()) {
            TreeItem * childItem = common::utils_qt::item<TreeItem*>(index);
            if(childItem){
                TreeItem * parentItem = childItem->parent();
                if (parentItem && parentItem != _root.get()) {
                    TreeItem * grandParent = parentItem->parent();
                    int row = grandParent->indexOf(parentItem);
                    result= createIndex(row, 0, parentItem);
                }
            }
        }
        return result;
    }

    TreeModel::~TreeModel()
    {

    }
}
