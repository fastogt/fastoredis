#pragma once

#include <QString>

#include "global/global.h"

namespace fastoredis
{
    namespace translations
    {
        extern const std::string defLanguage;
        QString applyLanguage(QString lang);
        QStringList getSupportedLanguages();
    }
}
