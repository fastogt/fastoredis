#include "core/events/events_info.h"

#include "common/time.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        EventInfoBase::EventInfoBase(const error_type &er)
            : base_class(er), time_start_(common::time::current_mstime())
        {

        }

        EventInfoBase::EventInfoBase(const common::time64_t time_start, const error_type &er)
            : base_class(er), time_start_(time_start)
        {

        }

        common::time64_t EventInfoBase::elapsedTime() const
        {
            return common::time::current_mstime() - time_start_;
        }

        ConnectInfoRequest::ConnectInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ConnectInfoResponce::ConnectInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ShutDownInfoRequest::ShutDownInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ShutDownInfoResponce::ShutDownInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        BackupInfoRequest::BackupInfoRequest(const std::string& path, const error_type &er)
            : base_class(er), path_(path)
        {

        }

        BackupInfoResponce::BackupInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ExportInfoRequest::ExportInfoRequest(const std::string& path, const error_type& er)
            : base_class(er), path_(path)
        {

        }

        ExportInfoResponce::ExportInfoResponce(const base_class &request, const error_type& er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ProcessConfigArgsInfoRequest::ProcessConfigArgsInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ProcessConfigArgsInfoResponce::ProcessConfigArgsInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        EnterModeInfo::EnterModeInfo(ConnectionMode mode, const error_type& er)
            : base_class(er), mode_(mode)
        {

        }

        LeaveModeInfo::LeaveModeInfo(ConnectionMode mode, const error_type& er)
            : base_class(er), mode_(mode)
        {

        }

        CommandRootCreatedInfo::CommandRootCreatedInfo(FastoObjectIPtr root, const error_type &er)
            : base_class(er), root_(root)
        {

        }

        CommandRootCompleatedInfo::CommandRootCompleatedInfo(FastoObjectIPtr root, const error_type &er)
            : base_class(er), root_(root)
        {

        }

        CommandRootCompleatedInfo::CommandRootCompleatedInfo(common::time64_t timest, FastoObjectIPtr root, const error_type &er)
            : base_class(timest, er), root_(root)
        {

        }

        DisonnectInfoRequest::DisonnectInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        DisConnectInfoResponce::DisConnectInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ExecuteInfoRequest::ExecuteInfoRequest(const std::string &text, const std::vector<std::string>& args, const error_type &er)
            : base_class(er), text_(text), args_(args)
        {

        }

        ExecuteScriptInfoRequest::ExecuteScriptInfoRequest(const std::string &path, const std::vector<std::string>& args, const error_type &er)
            : base_class(er), path_(path), args_(args)
        {

        }

        LoadDatabasesInfoRequest::LoadDatabasesInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        LoadDatabasesInfoResponce::LoadDatabasesInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

// ============== database =============//

        LoadDatabaseContentRequest::LoadDatabaseContentRequest(DataBaseInfoSPtr inf, const error_type &er)
            : base_class(er), inf_(inf)
        {

        }

        LoadDatabaseContentResponce::LoadDatabaseContentResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        SetDefaultDatabaseRequest::SetDefaultDatabaseRequest(DataBaseInfoSPtr inf, const error_type &er)
            : base_class(er), inf_(inf)
        {

        }

        SetDefaultDatabaseResponce::SetDefaultDatabaseResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

// ============== database =============//

        ServerInfoRequest::ServerInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ServerInfoResponce::ServerInfoResponce(const base_class &request, const error_type &er)
            : base_class(request), info_()
        {
            base_class::errorInfo_ = er;
        }

        ServerInfoSPtr ServerInfoResponce::info() const
        {
            return info_;
        }

        void ServerInfoResponce::setInfo(ServerInfoSPtr inf)
        {
            info_ = inf;
        }

        ServerInfoResponce::~ServerInfoResponce()
        {

        }

        ServerInfoHistoryRequest::ServerInfoHistoryRequest(const error_type& er)
            : base_class(er)
        {

        }

        ServerInfoHistoryResponce::ServerInfoHistoryResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ServerInfoHistoryResponce::infos_container_type ServerInfoHistoryResponce::infos() const
        {
            return infos_;
        }

        void ServerInfoHistoryResponce::setInfos(const infos_container_type& inf)
        {
            infos_ = inf;
        }

        ServerPropertyInfoRequest::ServerPropertyInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ServerPropertyInfoResponce::ServerPropertyInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

// ============== change =============//
        ChangeServerPropertyInfoRequest::ChangeServerPropertyInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ChangeServerPropertyInfoResponce::ChangeServerPropertyInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ChangeDbValueRequest::ChangeDbValueRequest(const error_type &er)
            : base_class(er)
        {

        }

        ChangeDbValueResponce::ChangeDbValueResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }
// ============== change =============//

// ============== command =============//

        CommandRequest::CommandRequest(DataBaseInfoSPtr inf, const CommandKey& cmd, const error_type& er)
            : base_class(er), inf_(inf), cmd_(cmd)
        {

        }

        CommandResponce::CommandResponce(const base_class& request, const error_type& er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

// ============== command =============//

        ProgressInfoResponce::ProgressInfoResponce(uint8_t pr)
            : progress_(pr)
        {

        }
    }
}
