#pragma once

/**/

#include <QAbstractItemModel>

#include "gui/base/tree_item.h"

namespace fastoredis
{
    class TreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        TreeModel(QObject *parent = 0);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent= QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& index) const;
        TreeItem* root() const;

        void setRoot(TreeItem *root);

        virtual void inserItem(const QModelIndex& parent, TreeItem* child);
        virtual void updateItem(const QModelIndex& topLeft, const QModelIndex &bottomRight);

        bool findItem(void* internalPointer, QModelIndex& index);

        virtual ~TreeModel();

    protected:        
        TreeItem* root_;
    };
}


