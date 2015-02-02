#include "gui/base/tree_item.h"

#include "common/macros.h"

namespace fastoredis
{
    TreeItem::TreeItem(TreeItem *parent, void* internalPointer)
        : parent_(parent), internalPointer_(internalPointer)
    {

    }

    TreeItem::~TreeItem()
    {
        clear();
    }

    void TreeItem::addChildren(TreeItem *child)
    {
        if(child){
            DCHECK(child->parent_ == this);
            childrens_.push_back(child);
        }
    }

    void TreeItem::removeChildren(TreeItem *child)
    {
        for(unsigned i = 0; i < childrens_.size(); ++i){
            if(child == childrens_[i]){
                childrens_.erase(childrens_.begin() + i);
            }
        }
    }

    void TreeItem::clear()
    {
        for(unsigned i = 0; i < childrens_.size(); ++i){
            delete childrens_[i];
        }
        childrens_.clear();
    }

    int TreeItem::childrenCount() const
    {
        return childrens_.size();
    }

    TreeItem *TreeItem::child(unsigned pos) const
    {
        if(pos < childrens_.size()){
            return childrens_[pos];
        }

        return 0;
    }

    int TreeItem::indexOf(TreeItem *item) const
    {
        for (unsigned i = 0; i < childrens_.size(); ++i) {
            if (item == childrens_[i]) {
                return i;
            }
        }
        return -1;
    }

    TreeItem *TreeItem::parent() const
    {
        return parent_;
    }

    void* const TreeItem::internalPointer() const
    {
        return internalPointer_;
    }
}

