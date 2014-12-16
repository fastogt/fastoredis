#include "core/events/events_info.h"

#include "common/time.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        EventInfoBase::EventInfoBase(const error_type &er)
            : base_class(er), _time_start(common::time::current_mstime())
        {

        }

        long long EventInfoBase::elapsedTime() const
        {
            return common::time::current_mstime() - _time_start;
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

        LoadDatabasesContentRequest::LoadDatabasesContentRequest(const DataBaseInfo &inf, const error_type &er)
            : base_class(er), inf_(inf)
        {

        }

        LoadDatabasesContentResponce::LoadDatabasesContentResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

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

        ChangeServerPropertyInfoRequest::ChangeServerPropertyInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        ChangeServerPropertyInfoResponce::ChangeServerPropertyInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ProgressInfoResponce::ProgressInfoResponce(uint8_t pr)
            : progress_(pr)
        {

        }
    }
}
