#include "gui/base/tree_model.h"

#include "common/macros.h"

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

            DCHECK(par);
            if(!par){
                return false;
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

    TreeModel::TreeModel(QObject* parent)
        : QAbstractItemModel(parent), root_(new TreeItem(NULL))
    {

    }

    TreeModel::~TreeModel()
    {
        delete root_;
        root_ = NULL;
    }

    int TreeModel::rowCount(const QModelIndex& parent) const
    {
        const TreeItem *parentItem=NULL;
        if (parent.isValid()){
            parentItem = common::utils_qt::item<TreeItem*>(parent);
        }
        else{
            parentItem = root_;
        }

        return parentItem ? parentItem->childrenCount() : 0;
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
    {
        if (hasIndex(row, column, parent)) {
            const TreeItem * parentItem = NULL;
            if (!parent.isValid()) {
                parentItem = root_;
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

        TreeItem* childItem = common::utils_qt::item<TreeItem*>(index);
        if(childItem){
            TreeItem* parentItem = childItem->parent();
            if (parentItem && parentItem != root_) {
                TreeItem* grandParent = parentItem->parent();
                if(grandParent){
                    int row = grandParent->indexOf(parentItem);
                    return createIndex(row, 0, parentItem);
                }
            }
        }

        return QModelIndex();
    }

    TreeItem* TreeModel::root() const
    {
        return root_;
    }

    void TreeModel::setRoot(TreeItem *root)
    {
        beginResetModel();
        delete root_;
        root_ = root;
        endResetModel();
    }

    void TreeModel::insertItem(const QModelIndex& parent, TreeItem* child)
    {
        TreeItem* item = NULL;
        if(!parent.isValid()){
            item = root_;
        }

        if(!item){
            item = common::utils_qt::item<TreeItem*>(parent);
        }

        DCHECK(item);
        if(!item){
            return;
        }

        int child_count = item->childrenCount();
        beginInsertRows(parent, child_count, child_count);
            item->addChildren(child);
        endInsertRows();
    }

    void TreeModel::removeItem(const QModelIndex& parent, TreeItem* child)
    {
        TreeItem* item = NULL;
        if(!parent.isValid()){
            item = root_;
        }

        if(!item){
            item = common::utils_qt::item<TreeItem*>(parent);
        }

        DCHECK(item);
        if(!item){
            return;
        }

        int child_count = item->childrenCount();
        beginRemoveRows(parent, item->indexOf(child), child_count);
            item->removeChildren(child);
            delete child;
        endRemoveRows();
    }

    void TreeModel::updateItem(const QModelIndex& topLeft, const QModelIndex& bottomRight)
    {
        emit dataChanged(topLeft, bottomRight);
    }

    bool TreeModel::findItem(void* internalPointer, QModelIndex& index)
    {
        return findChildInModel(QModelIndex(), internalPointer, index, root_, this);
    }
}
