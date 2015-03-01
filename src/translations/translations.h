#pragma once

#include <QString>

namespace fastoredis
{
    namespace translations
    {
        const QString defLanguage = "System";
        QString applyLanguage(const QString& lang);
        QStringList supportedLanguages();
    }
}
