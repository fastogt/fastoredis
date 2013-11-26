#pragma once

#include <QObject>
QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class RedisConnectionSettings;

    class RedisClient
            : public QObject
    {
        Q_OBJECT
    public:
        typedef QObject base_class;
        RedisClient(const RedisConnectionSettingsPtr &settings);

    protected:
        void customEvent(QEvent *event);

    private Q_SLOTS:
        void init();

    private:
        QThread *_thread;
        const RedisConnectionSettingsPtr settings_;
    };
}
