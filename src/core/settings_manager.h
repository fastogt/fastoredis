#pragma once

/**/

#include "global/global.h"

#include "common/patterns/singleton_pattern.h"

#include "core/connection_settings.h"

namespace fastoredis
{
    class SettingsManager
            : public common::patterns::lazy_singleton<SettingsManager>
    {
    public:
        typedef std::vector<IConnectionSettingsBasePtr> ConnectionSettingsContainerType;
        typedef common::patterns::lazy_singleton<SettingsManager> base_class;
        friend class common::patterns::lazy_singleton<SettingsManager>;

        void setDefaultView(supportedViews view);
        supportedViews defaultView() const;

        std::string currentStyle() const;
        void setCurrentStyle(const std::string& style);

        std::string currentLanguage() const;
        void setCurrentLanguage(const std::string& lang);

        void addConnection(const IConnectionSettingsBasePtr& connection);
        void removeConnection(const IConnectionSettingsBasePtr& connection);

        bool syncTabs() const;
        void setSyncTabs(bool sync);

        void setLoggingDirectory(const std::string& dir);
        std::string loggingDirectory() const;

        bool autoCheckUpdates() const;
        void setAutoCheckUpdates(bool isCheck);

        ConnectionSettingsContainerType connections() const;

        std::string pythonExecPath() const;
        void setPythonExecPath(const std::string& path);

    private:
        SettingsManager();
    };
}
