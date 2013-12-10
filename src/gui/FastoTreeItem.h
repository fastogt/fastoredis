#pragma once

#include <QString>

#include "global/global.h"
#include "gui/base/TreeItem.h"

namespace fastoredis
{
    class FastoTreeItem
            : public TreeItem
    {
    public:
        enum eColumn
        {
            eKey = 0,
            eValue = 1,
            eType = 2,
            eCountColumns = 3
        };
        typedef TreeItem base_class;
        typedef std::vector<FastoTreeItem*> child_container_type;
        FastoTreeItem(const QString &key, const QString &value, fastoType type, FastoTreeItem *parent = 0);

        QString key() const;
        QString value() const;
        fastoType type() const;
    private:

        QString key_;
        QString value_;
        fastoType type_;
    };
}

