#pragma once

#include "core/events/events_info.h"

namespace fastoredis
{
    namespace Events
    {
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoRequest, QEvent::User + 1> ConnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoResponce, QEvent::User + 2> ConnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ShutDownInfoRequest, QEvent::User + 3> ShutDownRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ShutDownInfoResponce, QEvent::User + 4> ShutDownResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ProcessConfigArgsInfoRequest, QEvent::User + 5> ProcessConfigArgsRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ProcessConfigArgsInfoResponce, QEvent::User + 6> ProcessConfigArgsResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::CommandRootCreatedInfo, QEvent::User + 7> CommandRootCreatedEvent;
        typedef common::utils_qt::Event<EventsInfo::CommandRootCompleatedInfo, QEvent::User + 8> CommandRootCompleatedEvent;

        typedef common::utils_qt::Event<EventsInfo::EnterModeInfo, QEvent::User + 9> EnterModeEvent;
        typedef common::utils_qt::Event<EventsInfo::LeaveModeInfo, QEvent::User + 10> LeaveModeEvent;

        typedef common::utils_qt::Event<EventsInfo::DisonnectInfoRequest, QEvent::User + 11> DisconnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::DisConnectInfoResponce, QEvent::User + 12> DisconnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ExecuteInfoRequest, QEvent::User + 13> ExecuteRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ExecuteScriptInfoRequest, QEvent::User + 14> ExecuteScriptRequestEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoRequest, QEvent::User + 15> LoadDatabasesInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoResponce, QEvent::User + 16> LoadDatabasesInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentRequest, QEvent::User + 17> LoadDatabaseContentRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentResponce, QEvent::User + 18> LoadDatabaseContentResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerInfoRequest, QEvent::User + 19> ServerInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerInfoResponce, QEvent::User + 20> ServerInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerInfoHistoryRequest, QEvent::User + 21> ServerInfoHistoryRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerInfoHistoryResponce, QEvent::User + 22> ServerInfoHistoryResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerPropertyInfoRequest, QEvent::User + 23> ServerPropertyInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerPropertyInfoResponce, QEvent::User + 24> ServerPropertyInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ChangeServerPropertyInfoRequest, QEvent::User + 25> ChangeServerPropertyInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ChangeServerPropertyInfoResponce, QEvent::User + 26> ChangeServerPropertyInfoResponceEvent;
        typedef common::utils_qt::Event<EventsInfo::ProgressInfoResponce, QEvent::User + 100> ProgressResponceEvent;
    }
}
