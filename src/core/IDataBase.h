#pragma once

#include <QObject>
#include <core/IServer.h>

namespace fastoredis
{
    class IDatabase
            : public QObject
    {
        Q_OBJECT
    public:
        typedef QObject base_class;
        IDatabase(IServer *parent);
    };
}
