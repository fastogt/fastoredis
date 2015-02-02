#include "gui/fasto_common_model.h"

#include "gui/fasto_common_item.h"
#include "gui/gui_factory.h"

#include "common/qt/utils_qt.h"
#include "common/qt/convert_string.h"
#include "translations/global.h"

namespace fastoredis
{
    FastoCommonModel::FastoCommonModel(QObject *parent)
        : TreeModel(parent)
    {

    }

    FastoCommonModel::~FastoCommonModel()
    {

    }

    QVariant FastoCommonModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        FastoCommonItem *node = common::utils_qt::item<FastoCommonItem*>(index);

        if (!node)
            return result;

        int col = index.column();

        if(role == Qt::DecorationRole && col == FastoCommonItem::eKey ){
            return GuiFactory::instance().icon(node->type());
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

    bool FastoCommonModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && role == Qt::EditRole) {
            int column = index.column();
            FastoCommonItem *node = common::utils_qt::item<FastoCommonItem*>(index);

            if (!node)
                return false;

            if (column == FastoCommonItem::eKey) {

            }
            else if (column == FastoCommonItem::eValue) {
                const QString newValue = value.toString();
                if(newValue != node->value()){
                    const std::string key = common::convertToString(node->key());
                    const std::string value = common::convertToString(newValue);
                    emit changeValue(DbValue(key, value));
                }
            }
        }

        return false;
    }

    QVariant FastoCommonModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        using namespace translations;
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == FastoCommonItem::eKey) {
                return trKey;
            }
            else if (section == FastoCommonItem::eValue) {
                return trValue;
            }
            else {
                return trType;
            }
        }

        return TreeModel::headerData(section,orientation,role);
    }

    int FastoCommonModel::columnCount(const QModelIndex &parent) const
    {
        return FastoCommonItem::eCountColumns;
    }

    Qt::ItemFlags FastoCommonModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags result = 0;
        if (index.isValid()) {
            result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
            int col = index.column();
            FastoCommonItem *node = common::utils_qt::item<FastoCommonItem*>(index);
            if(node && col == FastoCommonItem::eValue){
                result |= Qt::ItemIsEditable;
            }
        }
        return result;
    }
}
