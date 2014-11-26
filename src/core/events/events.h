#pragma once

/**/

#include "core/events/events_info.h"

namespace fastoredis
{
    namespace Events
    {
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoRequest, QEvent::User + 1> ConnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoResponce, QEvent::User + 2> ConnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ProcessConfigArgsInfoRequest, QEvent::User + 3> ProcessConfigArgsRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ProcessConfigArgsInfoResponce, QEvent::User + 4> ProcessConfigArgsResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::EnterModeInfo, QEvent::User + 5> EnterModeEvent;
        typedef common::utils_qt::Event<EventsInfo::LeaveModeInfo, QEvent::User + 6> LeaveModeEvent;

        typedef common::utils_qt::Event<EventsInfo::DisonnectInfoRequest, QEvent::User + 7> DisconnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::DisConnectInfoResponce, QEvent::User + 8> DisconnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ExecuteInfoRequest, QEvent::User + 9> ExecuteRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ExecuteInfoResponce, QEvent::User + 10> ExecuteResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoRequest, QEvent::User + 11> LoadDatabasesInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoResponce, QEvent::User + 12> LoadDatabasesInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentRequest, QEvent::User + 13> LoadDatabaseContentRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentResponce, QEvent::User + 14> LoadDatabaseContentResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerInfoRequest, QEvent::User + 15> ServerInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerInfoResponce, QEvent::User + 16> ServerInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerInfoHistoryRequest, QEvent::User + 17> ServerInfoHistoryRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerInfoHistoryResponce, QEvent::User + 18> ServerInfoHistoryResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerPropertyInfoRequest, QEvent::User + 19> ServerPropertyInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerPropertyInfoResponce, QEvent::User + 20> ServerPropertyInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ChangeServerPropertyInfoRequest, QEvent::User + 21> ChangeServerPropertyInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ChangeServerPropertyInfoResponce, QEvent::User + 22> ChangeServerPropertyInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ProgressInfoResponce, QEvent::User + 100> ProgressResponceEvent;
    }
}
