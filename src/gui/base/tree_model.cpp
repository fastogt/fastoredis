#include "gui/base/tree_model.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    namespace
    {
        bool findChildInModel(const QModelIndex& parent, void* internalPointer, QModelIndex& index, TreeItem* root, QAbstractItemModel* model)
        {
            if(!root || !model){
                return false;
            }

            TreeItem* par = NULL;
            if(!parent.isValid()){
                par = root;
            }
            else{
                par = common::utils_qt::item<TreeItem*>(parent);
            }

            if(par->internalPointer() == internalPointer){
                index = parent;
                return true;
            }

            for(int i = 0; i < par->childrenCount(); ++i){
                TreeItem* child = par->child(i);
                QModelIndex ind = model->index(i, 0, parent);
                if(child->internalPointer() == internalPointer){
                    index = ind;
                    return true;
                }
                else{
                    bool res = findChildInModel(ind, internalPointer, index, root, model);
                    if(res){
                        return true;
                    }
                }
            }

            return false;
        }
    }

    TreeModel::TreeModel(QObject *parent)
        : QAbstractItemModel(parent), _root(new TreeItem(NULL))
    {

    }

    TreeItem* TreeModel::root() const
    {
        return _root.get();
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

        return parentItem ? parentItem->childrenCount() : 0;
    }

    void TreeModel::setRoot(TreeItem *root)
    {
        beginResetModel();
        _root.reset(root);
        endResetModel();
    }

    void TreeModel::inserItem(const QModelIndex& parent, TreeItem* child)
    {
        TreeItem* item = NULL;
        if(!parent.isValid()){
            item = _root.get();
        }

        if(!item){
            item = common::utils_qt::item<TreeItem*>(parent);
        }

        int child_count = item->childrenCount();
        beginInsertRows(parent, child_count, child_count);
            item->addChildren(child);
        endInsertRows();
    }

    void TreeModel::updateItem(const QModelIndex &topLeft, const QModelIndex &bottomRight)
    {
        emit dataChanged(topLeft, bottomRight);
    }

    bool TreeModel::findItem(void *internalPointer, QModelIndex& index)
    {
        return findChildInModel(QModelIndex(), internalPointer, index, _root.get(), this);
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
    {
        if (hasIndex(row, column, parent)) {
            const TreeItem * parentItem = NULL;
            if (!parent.isValid()) {
                parentItem = _root.get();
            }
            else {
                parentItem = common::utils_qt::item<TreeItem*>(parent);
            }

            if(!parentItem){
                return QModelIndex();
            }

            TreeItem* childItem = parentItem->child(row);
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
