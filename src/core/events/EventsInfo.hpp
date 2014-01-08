#pragma once

#include "core/Infos.h"
#include "common/qt/utils_qt.h"
#include "common/time.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        struct EventInfoBase
                : public common::utils_qt::EventInfo<error::ErrorInfo>
        {
            typedef common::utils_qt::EventInfo<error::ErrorInfo> base_class;
            EventInfoBase(const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(er), _time_start(common::time::mstime()) {}
            long long elapsedTime() const { return common::time::mstime() - _time_start; }
            long long _time_start;
        };

        struct ConnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ConnectInfoRequest(const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct ConnectInfoResponce
                : ConnectInfoRequest
        {
            typedef ConnectInfoRequest base_class;
            ConnectInfoResponce(const base_class &request, const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(request)
            {
                errorInfo_ = er;
            }
        };

        struct DisonnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            DisonnectInfoRequest(const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct DisConnectInfoResponce
                : DisonnectInfoRequest
        {
            typedef DisonnectInfoRequest base_class;
            DisConnectInfoResponce(const base_class &request, const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(request)
            {
                base_class::errorInfo_ = er;
            }
        };

        struct ExecuteInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExecuteInfoRequest(const std::string &text, const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(er), _text(text)
            {

            }
            const std::string _text;
        };

        struct ExecuteInfoResponce
                : ExecuteInfoRequest
        {
            typedef ExecuteInfoRequest base_class;
            typedef FastoObjectPtr result_type;
            ExecuteInfoResponce(const base_class &request, const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(request), _out()
            {
                errorInfo_ = er;
            }
            result_type _out;
        };

        struct LoadDatabasesInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LoadDatabasesInfoRequest(const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct LoadDatabasesInfoResponce
                : LoadDatabasesInfoRequest
        {
            typedef LoadDatabasesInfoRequest base_class;
            typedef std::vector<DataBaseInfo> database_info_cont_type;
            LoadDatabasesInfoResponce(const base_class &request, const error::ErrorInfo &er = error::ErrorInfo())
                : base_class(request)
            {
                errorInfo_ = er;
            }
            database_info_cont_type databases_;
        };

        struct ProgressResponceInfo
        {
            ProgressResponceInfo(int pr)
                : _progress(pr){}

            const int _progress;
        };
    }
}
