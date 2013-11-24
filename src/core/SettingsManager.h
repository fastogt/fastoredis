#pragma once
#include <string>

#include "global/global.h"
#include "common/patterns/singleton_pattern.hpp"

namespace fastoredis
{
    class SettingsManager
            : public common::patterns::lazy_singleton<SettingsManager>
    {
    public:
        SettingsManager();
        typedef common::patterns::lazy_singleton<SettingsManager> base_class;
        friend class common::patterns::lazy_singleton<SettingsManager>;
        std::string currentStyle() const;
        void setCurrentStyle(const std::string &style) const;
    };
}
