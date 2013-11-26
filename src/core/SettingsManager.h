#pragma once

#include <string>
#include <vector>

#include "global/global.h"
#include "common/patterns/singleton_pattern.hpp"

#include "core/ConnectionSettings.h"

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

        std::string currentStyle() const;
        void setCurrentStyle(const std::string &style) const;

        void addConnection(const IConnectionSettingsBasePtr &con);
        ConnectionSettingsContainerType connections() const;
    };
}
