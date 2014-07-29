#pragma once

/**/

#include "global/global.h"

#include "core/events/infos.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        struct EventInfoBase
                : public common::utils_qt::EventInfo<common::ErrorValue>
        {
            typedef common::utils_qt::EventInfo<common::ErrorValue> base_class;
            EventInfoBase(const common::ErrorValue &er = common::ErrorValue());

            long long elapsedTime() const;
            long long _time_start;
        };

        struct ConnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ConnectInfoRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct ConnectInfoResponce
                : ConnectInfoRequest
        {
            typedef ConnectInfoRequest base_class;
            ConnectInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());
        };

        struct DisonnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            DisonnectInfoRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct DisConnectInfoResponce
                : DisonnectInfoRequest
        {
            typedef DisonnectInfoRequest base_class;
            DisConnectInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());
        };

        struct ExecuteInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExecuteInfoRequest(const std::string &text, const common::ErrorValue &er = common::ErrorValue());
            const std::string _text;
        };

        struct ExecuteInfoResponce
                : ExecuteInfoRequest
        {
            typedef ExecuteInfoRequest base_class;
            ExecuteInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            FastoObjectPtr _out;
        };

        struct LoadDatabasesInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LoadDatabasesInfoRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct LoadDatabasesInfoResponce
                : LoadDatabasesInfoRequest
        {
            typedef LoadDatabasesInfoRequest base_class;
            typedef std::vector<DataBaseInfo> database_info_cont_type;
            LoadDatabasesInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            database_info_cont_type databases_;
        };

        struct LoadDatabasesContentRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LoadDatabasesContentRequest(const DataBaseInfo &inf, const common::ErrorValue &er = common::ErrorValue());

            DataBaseInfo inf_;
        };

        struct LoadDatabasesContentResponce
                : LoadDatabasesContentRequest
        {
            typedef LoadDatabasesContentRequest base_class;
            LoadDatabasesContentResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());
        };

        struct ServerInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerInfoRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct ServerInfoResponce
                : ServerInfoRequest
        {
            typedef ServerInfoRequest base_class;
            ServerInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            ServerInfo info_;
        };

        struct ServerInfoHistoryRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerInfoHistoryRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct ServerInfoHistoryResponce
                : ServerInfoHistoryRequest
        {
            typedef ServerInfoHistoryRequest base_class;
            typedef std::map<long long, ServerInfo> infos_container_type;
            ServerInfoHistoryResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            infos_container_type infos_;
        };

        struct ServerPropertyInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerPropertyInfoRequest(const common::ErrorValue &er = common::ErrorValue());
        };

        struct ServerPropertyInfoResponce
                : ServerPropertyInfoRequest
        {
            typedef ServerPropertyInfoRequest base_class;
            ServerPropertyInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            ServerPropertyInfo info_;
        };

        struct ChangeServerPropertyInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ChangeServerPropertyInfoRequest(const common::ErrorValue &er = common::ErrorValue());

            PropertyType newItem_;
        };

        struct ChangeServerPropertyInfoResponce
                : ChangeServerPropertyInfoRequest
        {
            typedef ChangeServerPropertyInfoRequest base_class;
            ChangeServerPropertyInfoResponce(const base_class &request, const common::ErrorValue &er = common::ErrorValue());

            bool isChange_;
        };

        struct ProgressInfoResponce
        {
            ProgressInfoResponce(int pr);

            const int _progress;
        };
    }
}
