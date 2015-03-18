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
        IConnectionSettingsBaseSPtr connection() const;

    private:
        IConnectionSettingsBaseSPtr connection_;
    };

    class ConnectionListWidgetItemEx
            : public ConnectionListWidgetItem
    {
    public:
        ConnectionListWidgetItemEx(IConnectionSettingsBaseSPtr connection, bool isRoot);
        bool isRoot() const;

    private:
        const bool isRoot_;
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
