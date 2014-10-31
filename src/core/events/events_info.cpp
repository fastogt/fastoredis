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

        DisonnectInfoRequest::DisonnectInfoRequest(const error_type &er)
            : base_class(er)
        {

        }

        DisConnectInfoResponce::DisConnectInfoResponce(const base_class &request, const error_type &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ExecuteInfoRequest::ExecuteInfoRequest(const std::string &text, const error_type &er)
            : base_class(er), _text(text)
        {

        }

        ExecuteInfoResponce::ExecuteInfoResponce(const base_class &request, const error_type &er)
            : base_class(request), _out()
        {
            base_class::errorInfo_ = er;
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
            : base_class(request)
        {
            base_class::errorInfo_ = er;
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

        ProgressInfoResponce::ProgressInfoResponce(int pr)
            : _progress(pr)
        {

        }
    }
}
