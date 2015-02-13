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

        virtual int rowCount(const QModelIndex& parent) const;
        virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

        virtual ~TableModel();

    protected:
        std::vector<TableItem*> data_;
    };
}


