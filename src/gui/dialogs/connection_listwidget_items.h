#pragma once

#include <QTreeWidgetItem>

#include "core/connection_settings.h"

namespace fastoredis
{
    class ConnectionListWidgetItem
            : public QTreeWidgetItem
    {
    public:
        ConnectionListWidgetItem(IConnectionSettingsBaseSPtr connection);
        void setConnection(IConnectionSettingsBaseSPtr cons);
        IConnectionSettingsBaseSPtr connection() const;

    private:
        IConnectionSettingsBaseSPtr connection_;
    };

    class ClusterConnectionListWidgetItem
            : public QTreeWidgetItem
    {
    public:
        ClusterConnectionListWidgetItem(IClusterSettingsBaseSPtr connection);
        IClusterSettingsBaseSPtr connection() const;

    private:
        IClusterSettingsBaseSPtr connection_;
    };
}
