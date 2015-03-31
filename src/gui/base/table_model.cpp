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
        if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        TableItem *childItem = data_[row];
        if (childItem) {
            return createIndex(row, column, childItem);
        }
        return QModelIndex();
    }
}
