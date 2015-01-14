#pragma  once

#include <QDialog>

#include "core/events/events_info.h"

#include "core/redis/redis_infos.h"
#include "core/memcached/memcached_infos.h"

class QLabel;

namespace fastoredis
{
    class GlassWidget;
    class InfoServerDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit InfoServerDialog(const QString& title, connectionTypes type, QWidget* parent = 0);
        enum
        {
            min_height = 320,
            min_width = 240
        };

    Q_SIGNALS:
        void showed();

    public Q_SLOTS:
        void startServerInfo(const EventsInfo::ServerInfoRequest& req);
        void finishServerInfo(const EventsInfo::ServerInfoResponce& res);

    protected:
        virtual void showEvent(QShowEvent* e);

    private:
        void updateText(const RedisServerInfo& serv);
        void updateText(const MemcachedServerInfo& serv);
        QLabel* serverTextInfo_;
        QLabel* hardwareTextInfo_;
        GlassWidget* glassWidget_;
        const connectionTypes type_;
    };
}
