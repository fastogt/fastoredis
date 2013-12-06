#include "gui/FastoTreeView.h"

#include <QMenu>
#include <QHeaderView>

#include "common/macros.h"

namespace fastoredis
{
    FastoTreeView::FastoTreeView(QWidget *parent)
        :base_class(parent)
    {
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&))));
    }

    void FastoTreeView::showContextMenu(const QPoint &point)
    {
        QPoint menuPoint = mapToGlobal(point);
        menuPoint.setY(menuPoint.y() + header()->height());
        QMenu menu(this);
        menu.exec(menuPoint);
    }

}
