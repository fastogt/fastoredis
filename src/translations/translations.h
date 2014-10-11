#pragma once

/**/

#include <QString>

#include "common/types.h"

namespace fastoredis
{
    namespace translations
    {
        extern const std::string defLanguage;
        QString applyLanguage(const QString& lang);
        QStringList getSupportedLanguages();
    }
}
