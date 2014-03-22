#include "gui/base/tree_item.h"

#include <algorithm>

namespace fastoredis
{
    TreeItem::TreeItem(TreeItem *parent)
        : parent_(parent)
    {

    }

    void TreeItem::addChildren(TreeItem *child)
    {
        childrens_.push_back(child);
    }

    void TreeItem::removeChildren(TreeItem *child)
    {
        child_container_type::iterator it = std::find(childrens_.begin(),childrens_.end(),child);
        if (it != childrens_.end()) {
            childrens_.erase(it);
            delete child;
        }
    }

    int TreeItem::childrenCount() const
    {
        return childrens_.size();
    }

    TreeItem *TreeItem::child(unsigned pos) const
    {
        return childrens_[pos];
    }

    int TreeItem::indexOf(TreeItem *item) const
    {
        for (unsigned i=0; i < childrens_.size(); ++i) {
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

    TreeItem::~TreeItem()
    {
        for(unsigned i=0; i < childrens_.size(); ++i){
            delete childrens_[i];
        }
    }
}

