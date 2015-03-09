#pragma once

#include <QAbstractItemModel>

#include "gui/base/tree_item.h"

namespace fastoredis
{
    class TreeModel
            : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        TreeModel(QObject* parent = 0);
        virtual ~TreeModel();

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex& parent= QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex& index) const;

        TreeItem* root() const;
        void setRoot(TreeItem *root);

        virtual void insertItem(const QModelIndex& parent, TreeItem* child);
        virtual void removeItem(const QModelIndex& parent, TreeItem* child);
        virtual void updateItem(const QModelIndex& topLeft, const QModelIndex &bottomRight);

        bool findItem(void* internalPointer, QModelIndex& index);

    protected:        
        TreeItem* root_;
    };
}


