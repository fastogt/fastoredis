#pragma once

#include "common/qt_helper/utils_qt.h"
#include "core/events/ErrorInfo.hpp"

namespace fastoredis
{
    namespace EventsInfo
    {
        struct EventInfoBase
                : public common::utils_qt::EventInfo<Error::ErrorInfo>
        {
            typedef common::utils_qt::EventInfo<Error::ErrorInfo> base_class;
            EventInfoBase(const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(er){}
        };

        struct ConnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ConnectInfoRequest(const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct ConnectInfoResponce
                : ConnectInfoRequest
        {
            typedef ConnectInfoRequest base_class;
            ConnectInfoResponce(const base_class &request, const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(request)
            {
                base_class::errorInfo_ = er;
            }
            std::string _prompt;
        };

        struct DisonnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            DisonnectInfoRequest(const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct DisConnectInfoResponce
                : DisonnectInfoRequest
        {
            typedef DisonnectInfoRequest base_class;
            DisConnectInfoResponce(const base_class &request, const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(request)
            {
                base_class::errorInfo_ = er;
            }
        };

        struct ExecuteInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExecuteInfoRequest(const std::string &text, const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(er), _text(text)
            {

            }
            const std::string _text;
        };

        struct ExecuteInfoResponce
                : ExecuteInfoRequest
        {
            typedef ExecuteInfoRequest base_class;
            ExecuteInfoResponce(const base_class &request, const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(request)
            {
                base_class::errorInfo_ = er;
            }
        };

        struct InteruptInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            InteruptInfoRequest(const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(er)
            {

            }
        };

        struct InteruptInfoResponce
                : InteruptInfoRequest
        {
            typedef InteruptInfoRequest base_class;
            InteruptInfoResponce(const base_class &request, const Error::ErrorInfo &er = Error::ErrorInfo())
                : base_class(request)
            {
                base_class::errorInfo_ = er;
            }
            EventInfoBase _interuptedEvent;
        };
    }
}
