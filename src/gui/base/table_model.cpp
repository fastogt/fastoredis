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


    void TableModel::insertItem(TableItem* child)
    {
        if(!child){
            return;
        }

        beginInsertRows(QModelIndex(), data_.size(), data_.size());
        data_.push_back(child);
        endInsertRows();
    }

    void TableModel::removeItem(TableItem* child)
    {
        if(!child){
            return;
        }

        int child_count = data_.size();
        int index = -1;
        for(int i = 0; i < child_count; ++i){
            if(data_[i] == child){
                index = i;
                break;
            }
        }

        if(index == -1){
            return;
        }

        beginRemoveRows(QModelIndex(), index, child_count);
            data_.erase(data_.begin() + index);
            delete child;
        endRemoveRows();
    }

    void TableModel::updateItem(const QModelIndex& topLeft, const QModelIndex& bottomRight)
    {
        emit dataChanged(topLeft, bottomRight);
    }
}
