#pragma once

#include <QObject>
QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE
#include <boost/shared_ptr.hpp>

#include "core/ConnectionSettings.h"
#include "core/events/EventsInfo.hpp"

namespace fastoredis
{
    namespace detail
    {
        class Progresser
        {
        public:
            Progresser(QObject *sen, QObject *rec);
            ~Progresser();
        private:
            QObject *_sender;
            QObject *_reciver;
        };
    }

    class IDriver
            : public QObject
    {
        Q_OBJECT
    public:
        typedef QObject base_class;

        connectionTypes connectionType() const;
        const IConnectionSettingsBasePtr &settings() const;
        virtual ~IDriver();

        //sync
        virtual void interrupt() = 0;
        virtual bool isConnected() const = 0;

    private Q_SLOTS:
        void init();

    protected:
        IDriver(const IConnectionSettingsBasePtr &settings);
        virtual void customEvent(QEvent *event);
        void reply(QObject *reciver, QEvent *ev);
        virtual void initImpl() = 0;

        virtual void connectImpl(EventsInfo::ConnectInfoResponce &res) = 0;
        virtual void disconnectImpl(EventsInfo::DisConnectInfoResponce &res) = 0;
        virtual void executeImpl(EventsInfo::ExecuteInfoResponce &res) = 0;

    private:
        QThread *_thread;
        const IConnectionSettingsBasePtr _settings;        
    };

    typedef boost::shared_ptr<IDriver> IDriverPtr;
}
