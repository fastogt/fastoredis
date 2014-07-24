#pragma once

/**/

#include <vector>

namespace fastoredis
{

    class TreeItem
    {
    public:
        typedef std::vector<TreeItem*> child_container_type;
        TreeItem(TreeItem *parent);
        virtual ~TreeItem();

        void addChildren(TreeItem *child);
        void removeChildren(TreeItem *child);

        int childrenCount() const;
        TreeItem *child(unsigned pos) const;
        int indexOf(TreeItem *item) const;
        TreeItem *parent() const;

    protected:
        TreeItem *const parent_;

    private:
        child_container_type childrens_;        
    };
}

