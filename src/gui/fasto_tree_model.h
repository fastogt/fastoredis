#pragma once

/**/

#include "gui/base/tree_model.h"

namespace fastoredis
{
    class FastoCommonItem;

    class FastoTreeModel
            : public TreeModel
    {
        Q_OBJECT
    public:
        FastoTreeModel(QObject *parent = 0);
        ~FastoTreeModel();

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex& index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        virtual int columnCount(const QModelIndex& parent) const;
    };
}


