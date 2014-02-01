#pragma once

#include "core/events/EventsInfo.hpp"

namespace fastoredis
{
    namespace Events
    {
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoRequest, QEvent::User+1> ConnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ConnectInfoResponce, QEvent::User+2> ConnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::DisonnectInfoRequest, QEvent::User+3> DisconnectRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::DisConnectInfoResponce, QEvent::User+4> DisconnectResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ExecuteInfoRequest, QEvent::User+5> ExecuteRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ExecuteInfoResponce, QEvent::User+6> ExecuteResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoRequest, QEvent::User+7> LoadDatabasesInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesInfoResponce, QEvent::User+8> LoadDatabasesInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentRequest, QEvent::User+9> LoadDatabaseContentRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::LoadDatabasesContentResponce, QEvent::User+10> LoadDatabaseContentResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerInfoRequest, QEvent::User+11> ServerInfoRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerInfoResponce, QEvent::User+12> ServerInfoResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerPropertyRequest, QEvent::User+13> ServerPropertyRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerPropertyResponce, QEvent::User+14> ServerPropertyResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ServerPropertyChangeRequest, QEvent::User+15> ServerPropertyChangeRequestEvent;
        typedef common::utils_qt::Event<EventsInfo::ServerPropertyChangeResponce, QEvent::User+16> ServerPropertyChangeResponceEvent;

        typedef common::utils_qt::Event<EventsInfo::ProgressResponceInfo, QEvent::User+100> ProgressResponceEvent;
    }
}
