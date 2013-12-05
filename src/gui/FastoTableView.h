#pragma once

#include <QTableView>

namespace fastoredis
{
    class FastoTableView
            : public QTreeView
    {        
        Q_OBJECT
    public:
        typedef QTableView base_class;
        FastoTableView(QWidget *parent = 0);
    };
}

