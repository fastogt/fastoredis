#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

#include "global/global.h"

namespace fastoredis
{
    class ExplorerWidget 
            : public QWidget
    {
        Q_OBJECT

    public:
        typedef QWidget base_class;
        ExplorerWidget(QWidget *parent);

    private:
        QTreeView *treeView_;
    };
}
