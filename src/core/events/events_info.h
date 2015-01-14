#pragma once

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

            uint64_t elapsedTime() const;

        private:
            const uint64_t time_start_;
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

        struct ShutDownInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ShutDownInfoRequest(const error_type &er = error_type());
        };

        struct ShutDownInfoResponce
                : ShutDownInfoRequest
        {
            typedef ShutDownInfoRequest base_class;
            ShutDownInfoResponce(const base_class &request, const error_type &er = error_type());
        };

        struct BackupInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            BackupInfoRequest(const std::string& path, const error_type &er = error_type());
            std::string path_;
        };

        struct BackupInfoResponce
                : EventInfoBase
        {
            typedef EventInfoBase base_class;
            BackupInfoResponce(const base_class &request, const error_type &er = error_type());
        };

        struct ExportInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExportInfoRequest(const std::string& path, const error_type &er = error_type());
            std::string path_;
        };

        struct ExportInfoResponce
                : ExportInfoRequest
        {
            typedef ExportInfoRequest base_class;
            ExportInfoResponce(const base_class &request, const error_type &er = error_type());
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
            CommandRootCreatedInfo(FastoObjectIPtr root, const error_type &er = error_type());

            FastoObjectIPtr root_;
        };

        struct CommandRootCompleatedInfo
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            CommandRootCompleatedInfo(FastoObjectIPtr root, const error_type &er = error_type());

            FastoObjectIPtr root_;
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
            ExecuteInfoRequest(const std::string &text, const std::vector<std::string>& args = std::vector<std::string>(), const error_type &er = error_type());
            const std::string text_;
            const std::vector<std::string> args_;
        };

        struct ExecuteScriptInfoRequest
                : public EventInfoBase
        {
            typedef EventInfoBase base_class;
            ExecuteScriptInfoRequest(const std::string& path, const std::vector<std::string>& args, const error_type &er = error_type());

            const std::string path_;
            const std::vector<std::string> args_;
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
            typedef std::vector<ServerInfoSnapShoot> infos_container_type;
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
            ProgressInfoResponce(uint8_t pr);

            const uint8_t progress_;
        };
    }
}
