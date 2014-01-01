#pragma once

#include <QString>

#include "global/global.h"

namespace fastoredis
{
    namespace translations
    {
        extern const std::string defLanguage;
        namespace detail
        {
            bool applyLanguage(const QString &lang);
            QStringList getSupportedLanguages();
        }
    }
}
