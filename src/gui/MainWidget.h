#pragma once

#include <QTabWidget>

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class MainWidget
            : public QTabWidget
    {
        Q_OBJECT
    public:
        typedef QTabWidget base_class;
        explicit MainWidget(QWidget *parent=0);
        void addWidgetBySetting(const IConnectionSettingsBasePtr &setting);
    };
}
