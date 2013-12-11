#pragma once

#include "core/IServer.h"

namespace fastoredis
{
    class IServer;
    class IDatabase
    {
    public:
        typedef QObject base_class;
        QString name() const;

    protected:
        IDatabase(IServer *server, const QString &name);
        virtual ~IDatabase(){}
    private:
        IServer *parent_;
        QString name_;
    };
}
