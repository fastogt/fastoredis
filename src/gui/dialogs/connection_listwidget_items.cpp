#include "gui/dialogs/connection_listwidget_items.h"

#include "gui/gui_factory.h"

namespace fastoredis
{
    ConnectionListWidgetItem::ConnectionListWidgetItem(IConnectionSettingsBaseSPtr connection)
        : connection_(connection)
    {
        setText(0, common::convertFromString<QString>(connection_->connectionName()));
        connectionTypes conType = connection_->connectionType();
        setIcon(0, GuiFactory::instance().icon(conType));
        setText(1, common::convertFromString<QString>(connection_->fullAddress()));
    }

    IConnectionSettingsBaseSPtr ConnectionListWidgetItem::connection() const
    {
        return connection_;
    }

    ConnectionListWidgetItemEx::ConnectionListWidgetItemEx(IConnectionSettingsBaseSPtr connection, bool isRoot)
        : ConnectionListWidgetItem(connection), isRoot_(isRoot)
    {

    }

    bool ConnectionListWidgetItemEx::isRoot() const
    {
        return isRoot_;
    }

    ClusterConnectionListWidgetItem::ClusterConnectionListWidgetItem(IClusterSettingsBaseSPtr connection)
        : connection_(connection)
    {
        setText(0, common::convertFromString<QString>(connection_->connectionName()));
        setIcon(0, GuiFactory::instance().clusterIcon());

        IClusterSettingsBase::cluster_connection_type servers = connection_->nodes();

        for(int i = 0; i < servers.size(); ++i){
            IConnectionSettingsBaseSPtr con = servers[i];
            ConnectionListWidgetItem* item = new ConnectionListWidgetItem(con);
            addChild(item);
        }
    }

    IClusterSettingsBaseSPtr ClusterConnectionListWidgetItem::connection() const
    {
        return connection_;
    }
}
