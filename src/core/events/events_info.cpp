#include "core/events/events_info.h"

#include "common/time.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        EventInfoBase::EventInfoBase(const common::ErrorValue &er)
            : base_class(er), _time_start(common::time::current_mstime())
        {

        }

        long long EventInfoBase::elapsedTime() const
        {
            return common::time::current_mstime() - _time_start;
        }

        ConnectInfoRequest::ConnectInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        ConnectInfoResponce::ConnectInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        DisonnectInfoRequest::DisonnectInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        DisConnectInfoResponce::DisConnectInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ExecuteInfoRequest::ExecuteInfoRequest(const std::string &text, const common::ErrorValue &er)
            : base_class(er), _text(text)
        {

        }

        ExecuteInfoResponce::ExecuteInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request), _out()
        {
            base_class::errorInfo_ = er;
        }

        LoadDatabasesInfoRequest::LoadDatabasesInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        LoadDatabasesInfoResponce::LoadDatabasesInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        LoadDatabasesContentRequest::LoadDatabasesContentRequest(const DataBaseInfo &inf, const common::ErrorValue &er)
            : base_class(er), inf_(inf)
        {

        }

        LoadDatabasesContentResponce::LoadDatabasesContentResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ServerInfoRequest::ServerInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        ServerInfoResponce::ServerInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ServerPropertyInfoRequest::ServerPropertyInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        ServerPropertyInfoResponce::ServerPropertyInfoResponce(const base_class &request, const common::ErrorValue &er)
            : base_class(request)
        {
            base_class::errorInfo_ = er;
        }

        ChangeServerPropertyInfoRequest::ChangeServerPropertyInfoRequest(const common::ErrorValue &er)
            : base_class(er)
        {

        }

        ChangeServerPropertyInfoResponce::ChangeServerPropertyInfoResponce(const base_class &request, const common::ErrorValue &er)
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
