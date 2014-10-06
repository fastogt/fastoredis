#pragma once

/**/

#include <QString>

#include "common/types.h"

namespace fastoredis
{
    namespace translations
    {
        extern const common::string16 defLanguage;
        QString applyLanguage(const QString& lang);
        QStringList getSupportedLanguages();
    }
}
