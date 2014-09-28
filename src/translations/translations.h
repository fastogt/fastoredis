#pragma once

/**/

#include <QString>

#include "common/types.h"

namespace fastoredis
{
    namespace translations
    {
        extern const unicode_string defLanguage;
        QString applyLanguage(const QString& lang);
        QStringList getSupportedLanguages();
    }
}
