#pragma once

#include <boost/scoped_ptr.hpp>
#include "core/IDriver.h"

namespace fastoredis
{
    class RedisDriver
            : public IDriver
    {
        Q_OBJECT
    public:
        typedef IDriver base_class;
        RedisDriver(const IConnectionSettingsBasePtr &settings);
        virtual ~RedisDriver();

        virtual bool isConnected() const;
        virtual void interrupt();
        std::string adress() const;

        static QStringList allCommands();
    protected:
        void customEvent(QEvent *event);
        virtual void initImpl();

        virtual void connectEvent(Events::ConnectRequestEvent *ev);
        virtual void disconnectEvent(Events::DisconnectRequestEvent *ev);
        virtual void executeEvent(Events::ExecuteRequestEvent *ev);

    private:
        struct pimpl;
        boost::scoped_ptr<pimpl> _impl;
    };
}
