#pragma once

#include "core/IServer.h"

namespace fastoredis
{
    class IDatabase
    {
    public:
        QString name() const;
        IServerPtr server() const;

    protected:
        IDatabase(const IServerPtr &server, const QString &name);
        virtual ~IDatabase();

    private:
        IServerPtr server_;
        QString name_;
    };
}
