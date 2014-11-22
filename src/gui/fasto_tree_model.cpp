#include "gui/fasto_tree_model.h"

#include "gui/fasto_common_item.h"
#include "gui/gui_factory.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    FastoTreeModel::FastoTreeModel(QObject *parent)
        : TreeModel(parent)
    {

    }

    FastoTreeModel::~FastoTreeModel()
    {

    }

    void FastoTreeModel::setRoot(FastoCommonItem *root)
    {
        beginResetModel();
        _root.reset(root);
        endResetModel();
    }

    QVariant FastoTreeModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        FastoCommonItem *node = common::utils_qt::item<FastoCommonItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if(role == Qt::DecorationRole && col == FastoCommonItem::eKey ){
            return GuiFactory::instance().getIcon(node->type());
        }

        if(role == Qt::TextColorRole && col == FastoCommonItem::eType){
            return QColor(Qt::gray);
        }

        if (role == Qt::DisplayRole) {
            if (col == FastoCommonItem::eKey) {
                result = node->key();
            }
            else if (col == FastoCommonItem::eValue) {
                result = node->value();
            }
            else if (col == FastoCommonItem::eType) {
                result = common::convertFromString<QString>(common::Value::toString(node->type()));
            }
        }

        return result;
    }

    QVariant FastoTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == FastoCommonItem::eKey) {
                return "Key";
            }
            else if (section == FastoCommonItem::eValue) {
                return "Value";
            }
            else {
                return "Type";
            }
        }

        return TreeModel::headerData(section,orientation,role);
    }

    int FastoTreeModel::columnCount(const QModelIndex &parent) const
    {
        return FastoCommonItem::eCountColumns;
    }

    Qt::ItemFlags FastoTreeModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags result = 0;
        if (index.isValid()) {
            result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        return result;
    }
}
