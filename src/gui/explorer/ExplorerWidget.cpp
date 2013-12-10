#include "gui/explorer/ExplorerWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QTreeView>

#include "gui/explorer/ExplorerTreeModel.h"
#include "gui/explorer/ExplorerTreeModel.h"

namespace fastoredis
{
    ExplorerWidget::ExplorerWidget(QWidget *parent)
        : base_class(parent)
    {
        treeView_ = new QTreeView;
        treeView_->setModel(new ExplorerTreeModel(treeView_));
        QHBoxLayout *ml = new QHBoxLayout;
        ml->addWidget(treeView_);
        setLayout(ml);
    }
}
