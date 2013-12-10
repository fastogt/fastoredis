#pragma once

#include "gui/base/TreeModel.h"

namespace fastoredis
{
    class FastoTreeItem;

    class FastoTreeModel
            : public TreeModel
    {
        Q_OBJECT
    public:
        typedef TreeModel base_class;
        FastoTreeModel(QObject *parent = 0);
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        virtual int columnCount(const QModelIndex &parent) const;

        void setRoot(FastoTreeItem *root);
        ~FastoTreeModel();
    };
}


