#pragma once

#include <QObject>
QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE
#include <boost/shared_ptr.hpp>

#include "core/ConnectionSettings.h"
#include "core/events/Events.hpp"

namespace fastoredis
{
    class IDriver
            : public QObject
    {
        Q_OBJECT
    public:
        typedef QObject base_class;

        static void reply(QObject *reciver, QEvent *ev);

        connectionTypes connectionType() const;
        const IConnectionSettingsBasePtr &settings() const;
        virtual ~IDriver();

        //sync
        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;
        virtual std::string adress() const = 0;

    private Q_SLOTS:
        void init();

    protected:
        IDriver(const IConnectionSettingsBasePtr &settings);
        void notifyProgress(QObject *reciver, int value);

        virtual void customEvent(QEvent *event);
        virtual void initImpl() = 0;

        virtual void connectEvent(Events::ConnectRequestEvent *ev) = 0;
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev) = 0;
        virtual void executeEvent(Events::ExecuteRequestEvent *ev) = 0;

    private:
        QThread *_thread;

    protected:
        const IConnectionSettingsBasePtr _settings;        
    };

    typedef boost::shared_ptr<IDriver> IDriverPtr;
}
