#include "gui/base/table_model.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    TableModel::TableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {

    }

    TableModel::~TableModel()
    {

    }

    int TableModel::rowCount(const QModelIndex& parent) const
    {
        int sz = data_.size();
        return sz;
    }


    QModelIndex TableModel::index(int row, int column, const QModelIndex& parent) const
    {
        QModelIndex index;
        if (hasIndex(row, column, parent)) {
            TableItem *childItem = data_[row];
            if (childItem) {
                index = createIndex(row, column, childItem);
            }
        }
        return index;
    }
}
