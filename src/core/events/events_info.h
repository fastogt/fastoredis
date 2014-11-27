#pragma once

/**/

#include "global/global.h"

#include "core/types.h"

#include "common/qt/utils_qt.h"

namespace fastoredis
{
    namespace EventsInfo
    {
        struct EventInfoBase
                : public common::utils_qt::EventInfo<common::ErrorValueSPtr>
        {
            typedef common::utils_qt::EventInfo<common::ErrorValueSPtr > base_class;
            EventInfoBase(const error_type &er = error_type());

            long long elapsedTime() const;
            long long _time_start;
        };

        struct ConnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ConnectInfoRequest(const error_type &er = error_type());
        };

        struct ConnectInfoResponce
                : ConnectInfoRequest
        {
            typedef ConnectInfoRequest base_class;
            ConnectInfoResponce(const base_class &request, const error_type &er = error_type());
        };

        struct ProcessConfigArgsInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ProcessConfigArgsInfoRequest(const error_type &er = error_type());
        };

        struct ProcessConfigArgsInfoResponce
                : ProcessConfigArgsInfoRequest
        {
            typedef ProcessConfigArgsInfoRequest base_class;
            ProcessConfigArgsInfoResponce(const base_class &request, const error_type &er = error_type());
        };

        struct EnterModeInfo
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            EnterModeInfo(ConnectionMode mode, const error_type &er = error_type());
            ConnectionMode mode_;
        };

        struct LeaveModeInfo
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LeaveModeInfo(ConnectionMode mode, const error_type &er = error_type());
            ConnectionMode mode_;
        };

        struct CommandRootCreatedInfo
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            CommandRootCreatedInfo(FastoObjectPtr root, const error_type &er = error_type());

            FastoObjectPtr root_;
        };

        struct CommandRootAddedChildInfo
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            CommandRootAddedChildInfo(FastoObjectPtr root, FastoObject* child, const error_type &er = error_type());

            FastoObjectPtr root_;
            FastoObject* child_;
        };

        struct DisonnectInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            DisonnectInfoRequest(const error_type &er = error_type());
        };

        struct DisConnectInfoResponce
                : DisonnectInfoRequest
        {
            typedef DisonnectInfoRequest base_class;
            DisConnectInfoResponce(const base_class &request, const error_type &er = error_type());
        };

        struct ExecuteInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExecuteInfoRequest(const std::string &text, const error_type &er = error_type());
            const std::string _text;
        };

        struct ExecuteInfoResponce
                : ExecuteInfoRequest
        {
            typedef ExecuteInfoRequest base_class;
            ExecuteInfoResponce(const base_class &request, const error_type &er = error_type());

            FastoObjectPtr out_;
        };

        struct LoadDatabasesInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LoadDatabasesInfoRequest(const error_type &er = error_type());
        };

        struct LoadDatabasesInfoResponce
                : LoadDatabasesInfoRequest
        {
            typedef LoadDatabasesInfoRequest base_class;
            typedef std::vector<DataBaseInfo> database_info_cont_type;
            LoadDatabasesInfoResponce(const base_class &request, const error_type &er = error_type());

            database_info_cont_type databases_;
        };

        struct LoadDatabasesContentRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            LoadDatabasesContentRequest(const DataBaseInfo &inf, const error_type &er = error_type());

            DataBaseInfo inf_;
        };

        struct LoadDatabasesContentResponce
                : LoadDatabasesContentRequest
        {
            typedef LoadDatabasesContentRequest base_class;
            LoadDatabasesContentResponce(const base_class &request, const error_type &er = error_type());
        };

        struct ServerInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerInfoRequest(const error_type &er = error_type());
        };

        struct ServerInfoResponce
                : ServerInfoRequest
        {
            typedef ServerInfoRequest base_class;
            ServerInfoResponce(const base_class &request, const error_type &er = error_type());
            ~ServerInfoResponce();

            ServerInfoSPtr info() const;
            void setInfo(ServerInfoSPtr inf);

        private:
            ServerInfoSPtr info_;
        };

        struct ServerInfoHistoryRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerInfoHistoryRequest(const error_type &er = error_type());
        };

        struct ServerInfoHistoryResponce
                : ServerInfoHistoryRequest
        {
            typedef ServerInfoHistoryRequest base_class;
            typedef std::map<long long, ServerInfoSPtr > infos_container_type;
            ServerInfoHistoryResponce(const base_class &request, const error_type &er = error_type());

            infos_container_type infos() const;
            void setInfos(const infos_container_type& inf);

        private:
            infos_container_type infos_;
        };

        struct ServerPropertyInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ServerPropertyInfoRequest(const error_type &er = error_type());
        };

        struct ServerPropertyInfoResponce
                : ServerPropertyInfoRequest
        {
            typedef ServerPropertyInfoRequest base_class;
            ServerPropertyInfoResponce(const base_class &request, const error_type &er = error_type());

            ServerPropertyInfo info_;
        };

        struct ChangeServerPropertyInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ChangeServerPropertyInfoRequest(const error_type &er = error_type());

            PropertyType newItem_;
        };

        struct ChangeServerPropertyInfoResponce
                : ChangeServerPropertyInfoRequest
        {
            typedef ChangeServerPropertyInfoRequest base_class;
            ChangeServerPropertyInfoResponce(const base_class &request, const error_type &er = error_type());

            bool isChange_;
        };

        struct ProgressInfoResponce
        {
            ProgressInfoResponce(int pr);

            const int _progress;
        };
    }
}
