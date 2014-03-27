#pragma once

#include "global/global.h"
#include "common/patterns/singleton_pattern.hpp"

#include "core/connection_settings.h"

namespace fastoredis
{
    class IConnectionSettingsBase;
    class SettingsManager
            : public common::patterns::lazy_singleton<SettingsManager>
    {
    public:
        typedef std::vector< IConnectionSettingsBasePtr > ConnectionSettingsContainerType;

        SettingsManager();
        typedef common::patterns::lazy_singleton<SettingsManager> base_class;
        friend class common::patterns::lazy_singleton<SettingsManager>;

        void setDefaultView(supportedViews view);
        supportedViews defaultView() const;

        std::string currentStyle() const;
        void setCurrentStyle(const std::string &style) const;

        std::string currentLanguage() const;
        void setCurrentLanguage(const std::string &lang) const;

        void addConnection(const IConnectionSettingsBasePtr &connection);
        void removeConnection(const IConnectionSettingsBasePtr &connection);

        bool syncTabs() const;
        void setSyncTabs(bool sync) const;

        ConnectionSettingsContainerType connections() const;
    };
}
