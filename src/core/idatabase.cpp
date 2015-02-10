#include "core/idatabase.h"

#include "core/iserver.h"

namespace fastoredis
{
    IDatabase::IDatabase(IServerSPtr server, DataBaseInfoSPtr info)
        : server_(server), info_(info)
    {
        DCHECK(server);
        DCHECK(info);
    }

    connectionTypes IDatabase::type() const
    {
        return server_->type();
    }

    IServerSPtr IDatabase::server() const
    {
        return server_;
    }

    bool IDatabase::isDefault() const
    {
        return info_->isDefault();
    }

    std::string IDatabase::name() const
    {
        return info_->name();
    }

    void IDatabase::loadContent()
    {
        server_->loadDatabaseContent(info_);
    }

    void IDatabase::setDefault()
    {
        server_->setDefaultDb(info_);
    }

    DataBaseInfoSPtr IDatabase::info() const
    {
        return info_;
    }

    void IDatabase::setInfo(DataBaseInfoSPtr info)
    {
        info_ = info;
    }

    void IDatabase::removeKey(const std::string& key)
    {
        server_->executeCommand(info_, CommandKey(key, CommandKey::C_DELETE));
    }

    void IDatabase::loadValue(const std::string& key)
    {
        server_->executeCommand(info_, CommandKey(key, CommandKey::C_LOAD));
    }

    IDatabase::~IDatabase()
    {

    }
}
