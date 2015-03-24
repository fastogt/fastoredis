#pragma once

#include <QString>

namespace fastoredis
{
    namespace translations
    {
        extern const QString defLanguage;
        QString applyLanguage(const QString& lang);
        QStringList supportedLanguages();
    }
}
