#include "translations/translations.h"

#include <QTranslator>
#include <QDir>
#include <QApplication>

#include "common/qt/convert_string.h"
#include "common/qt/utils_qt.h"

namespace
{
    QString trPath()
    {
        return common::utils_qt::applicationDirPath() + "/translations";
    }

    QStringList qmLanguages()
    {
        static QDir trd(trPath());
        return trd.entryList(QStringList(PROJECT_NAME_LOWERCASE"_*.qm"));
    }

    QPair<QString, QLocale> convertToLocale(const QString &fileName)
    {
        QString langCode = fileName;
        langCode.remove(0, fileName.indexOf('_') + 1);
        langCode.chop(3);
        return QPair<QString, QLocale>(fileName, langCode);
    }

    QString pathToQm(const QString &l)
    {
        const QStringList languages = qmLanguages();
        for(int i = 0; i < languages.size(); ++i){
            QPair<QString, QLocale> p = convertToLocale(languages[i]);
            QString lang = QLocale::languageToString(p.second.language());
            if(l == lang){
                return p.first;
            }
        }
        return QString();
    }

    const QString builtInLanguage = "English"; /* "English" is built-in */
}

namespace fastoredis
{
    namespace translations
    {
        QString applyLanguage(const QString &lang)
        {
            QString langres = lang;

            static QTranslator tr;
            if(tr.isEmpty()){
                qApp->installTranslator(&tr);
            }

            if(langres == defLanguage){
                langres = QLocale::languageToString(QLocale::system().language());
            }

            QString qmPath = pathToQm(langres);
            bool isLoad = tr.load(qmPath, trPath());

            if(!isLoad && langres != builtInLanguage){
                return builtInLanguage;
            }

            return langres;
        }

        QStringList getSupportedLanguages()
        {
            const QStringList languages = qmLanguages();

            QStringList result;
            result << defLanguage << builtInLanguage;
            for(int i = 0; i < languages.size(); ++i){
                QPair<QString,QLocale> p = convertToLocale(languages[i]);
                QString lang = QLocale::languageToString(p.second.language());
                result.append(lang);
            }
            return result;
        }
    }
}
