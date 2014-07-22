#pragma once

/**/

#include <vector>

namespace fastoredis
{

    class TreeItem
    {
    public:
        typedef std::vector<TreeItem*> child_container_type;
        TreeItem(TreeItem *parent = 0);

        void addChildren(TreeItem *child);
        void removeChildren(TreeItem *child);

        void removeChildren(int index);

        int childrenCount() const;
        TreeItem *child(unsigned pos) const;
        int indexOf(TreeItem *item) const;
        TreeItem *parent() const;

        virtual ~TreeItem();
    protected:
        TreeItem *parent_;
        child_container_type childrens_;        
    };
}

