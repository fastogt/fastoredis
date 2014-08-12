#pragma once

/**/

#include <QDialog>

#include "core/events/events_info.h"
#include "core/connection_types.h"

namespace fastoredis
{
    class GraphWidget;
    class GlassWidget;

    class ServerHistoryDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit ServerHistoryDialog(const QString &title, connectionTypes type, QWidget *parent = 0);

    Q_SIGNALS:
        void showed();

    public Q_SLOTS:
        void startLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &req);
        void finishLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &res);

    protected:
        virtual void showEvent(QShowEvent *e);

    private:
        GraphWidget* graphWidget_;
        GlassWidget *glassWidget_;
        EventsInfo::ServerInfoHistoryResponce::infos_container_type infos_;
        const connectionTypes type_;
    };
}
