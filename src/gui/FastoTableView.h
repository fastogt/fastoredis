#pragma once

#include <QTableView>

namespace fastoredis
{
    class FastoTableView
            : public QTableView
    {        
        Q_OBJECT
    public:
        typedef QTableView base_class;
        FastoTableView(QWidget *parent = 0);
    };
}

