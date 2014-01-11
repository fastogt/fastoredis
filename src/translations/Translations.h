#pragma once

#include <QString>

namespace fastoredis
{
    namespace translations
    {
        extern const std::string defLanguage;
        QString applyLanguage(QString lang);
        QStringList getSupportedLanguages();
    }
}
