#include "gui/fasto_table_model.h"

#include <QBrush>
#include <QIcon>

#include "gui/fasto_tree_item.h"
#include "gui/gui_factory.h"

#include "common/qt/convert_string.h"
#include "common/qt/utils_qt.h"

namespace
{
    struct KDPrivateModelIndex
    {
        int r, c;
        void *p;
        const QAbstractItemModel *m;
    };
}

namespace fastoredis
{
    FastoTableModel::FastoTableModel(QObject *parent)
        :QAbstractProxyModel(parent)
    {

    }

    QModelIndex FastoTableModel::mapFromSource( const QModelIndex & sourceIndex ) const
    {
        return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer() );
    }

    QModelIndex FastoTableModel::mapToSource( const QModelIndex &proxyIndex ) const
    {
            if ( !proxyIndex.isValid() )
                    return QModelIndex();

            Q_ASSERT( proxyIndex.model() == this );

            QModelIndex sourceIndex;
            KDPrivateModelIndex* hack = reinterpret_cast<KDPrivateModelIndex*>(&sourceIndex);
            hack->r = proxyIndex.row();
            hack->c = proxyIndex.column();
            hack->p = proxyIndex.internalPointer();
            hack->m = sourceModel();
            return sourceIndex;
    }

    QVariant FastoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole){
                    return QVariant();
        }

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if(section == FastoTreeItem::eKey){
                return "Key";
            }
            else if(section == FastoTreeItem::eValue){
                return "Value";
            }
            else if(section == FastoTreeItem::eType){
                return "Type";
            }
        } else {
            return QString("%1").arg(section + 1);
        }

        return QVariant(); 
    }

    QModelIndex FastoTableModel::index( int row, int col, const QModelIndex& index ) const
    {
        return mapFromSource(sourceModel()->index( row, col, mapToSource(index) ));
    }

    QModelIndex FastoTableModel::parent( const QModelIndex& index ) const
    {
        return mapFromSource(sourceModel()->parent( mapToSource(index) ));
    }

    int FastoTableModel::rowCount(const QModelIndex &parent) const
    {
        int count = sourceModel()->rowCount(parent);
        return count;
    }

    int FastoTableModel::columnCount(const QModelIndex &parent) const
    {
        return FastoTreeItem::eCountColumns;
    }

    QVariant FastoTableModel::data(const QModelIndex &index, int role) const
    {
        QVariant result;

        if (!index.isValid())
            return result;

        FastoTreeItem *node = common::utils_qt::item<FastoTreeItem*>(index);

        if (!node) {
            return result;
        }

        int col = index.column();

        if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
            if (col == FastoTreeItem::eKey) {
                result = node->key();
            }
            else if (col == FastoTreeItem::eValue) {
                result = node->value();
            }
            else if (col == FastoTreeItem::eType) {
                result = common::convertfromString<QString>(common::Value::toString(node->type()));
            }
        }
        else if (role == Qt::DecorationRole) {
            return GuiFactory::instance().getIcon(node->type());
        }

        return result;
    }
}
