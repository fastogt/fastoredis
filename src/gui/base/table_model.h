#pragma once

#include <QAbstractTableModel>

#include "gui/base/table_item.h"

namespace fastoredis
{
    class TableModel
            : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        TableModel(QObject* parent = 0);
        virtual ~TableModel();

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

        virtual void insertItem(TableItem* child);
        virtual void removeItem(TableItem* child);
        virtual void updateItem(const QModelIndex& topLeft, const QModelIndex &bottomRight);

    protected:
        std::vector<TableItem*> data_;
    };
}


