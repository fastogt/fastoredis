#pragma  once

#include <QDialog>
#include "core/events/EventsInfo.hpp"

namespace fastoredis
{
    class InfoServerDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit InfoServerDialog(QWidget *parent = 0);

    Q_SIGNALS:
        void showed();

    public Q_SLOTS:
        void startServerInfo(const EventsInfo::ServerInfoRequest &req);
        void finishServerInfo(const EventsInfo::ServerInfoResponce &res);

    protected:
        virtual void showEvent(QShowEvent *e);
    };
}
