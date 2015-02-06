#pragma once

#include "core/core_fwd.h"

namespace fastoredis
{
    class IDatabase
    {
    public:
        virtual ~IDatabase();

        connectionTypes type() const;
        IServerSPtr server() const;
        bool isDefault() const;
        std::string name() const;

        void loadContent();
        void setDefault();

        DataBaseInfoSPtr info() const;
        void setInfo(DataBaseInfoSPtr info);

    protected:
        IDatabase(IServerSPtr server, DataBaseInfoSPtr info);

        DataBaseInfoSPtr info_;
        const IServerSPtr server_;
    };
}
