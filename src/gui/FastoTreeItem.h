#pragma once

#include <QString>

#include "global/global.h"

namespace fastoredis
{
    class FastoTreeItem
    {
    public:
        enum eColumn
        {
            eKey = 0,
            eValue = 1,
            eType = 2,
            eCountColumns = 3
        };
        typedef std::vector<FastoTreeItem*> child_container_type;
        FastoTreeItem(const QString &key, const QString &value, fastoType type, FastoTreeItem *parent = 0);

        QString key() const;
        QString value() const;
        fastoType type() const;
        void addChildren(FastoTreeItem *child);
        int childrenCount() const;

        FastoTreeItem *child(unsigned pos) const;
        int indexOf(FastoTreeItem *item) const;
        FastoTreeItem *parent() const;

        ~FastoTreeItem();
    private:

        FastoTreeItem *parent_;
        child_container_type childrens_;

        QString key_;
        QString value_;
        fastoType type_;
    };
}

