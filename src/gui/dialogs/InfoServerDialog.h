#pragma  once

#include <QDialog>

#include "core/events/EventsInfo.hpp"
#include "core/ConnectionTypes.h"

class QLabel;

namespace fastoredis
{
    class InfoServerDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit InfoServerDialog(connectionTypes type, QWidget *parent = 0);

    Q_SIGNALS:
        void showed();

    public Q_SLOTS:
        void startServerInfo(const EventsInfo::ServerInfoRequest &req);
        void finishServerInfo(const EventsInfo::ServerInfoResponce &res);

    protected:
        virtual void showEvent(QShowEvent *e);

    private:
        void updateText(const ServerInfo &serv);
        QLabel *serverTextInfo_;
        QLabel *hardwareTextInfo_;
        const connectionTypes type_;
    };
}
