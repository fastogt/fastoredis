#include "gui/base/tree_item.h"

#include <algorithm>

namespace fastoredis
{
    TreeItem::TreeItem(TreeItem *parent)
        : parent_(parent)
    {

    }

    TreeItem::~TreeItem()
    {
        for(unsigned i = 0; i < childrens_.size(); ++i){
            delete childrens_[i];
        }
        childrens_.clear();
    }

    void TreeItem::addChildren(TreeItem *child)
    {
        childrens_.push_back(child);
    }

    void TreeItem::removeChildren(TreeItem *child)
    {
        for(unsigned i = 0; i < childrens_.size(); ++i){
            if(child == childrens_[i]){
                childrens_.erase(childrens_.begin() + i);
            }
        }
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
        return NULL;
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
}

