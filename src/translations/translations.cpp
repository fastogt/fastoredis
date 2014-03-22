#include "translations/translations.h"

#include <QTranslator>
#include <QApplication>
#include <QDir>

#include "common/qt/convert_string.h"

namespace
{
    QString trPath()
    {
        return QApplication::applicationDirPath() + "/translations";
    }

    QStringList qmLanguages()
    {
        static QDir trd(trPath());
        return trd.entryList(QStringList(PROJECT_NAME"_*.qm"));
    }

    QPair<QString,QLocale> convertToLocale(const QString &fileName)
    {
        QString langCode = fileName;
        langCode.remove(0, fileName.indexOf('_')+1);
        langCode.chop(3);
        return QPair<QString,QLocale>(fileName, langCode);
    }

    QString pathToQm(const QString &l)
    {
        const QStringList languages = qmLanguages();
        for(int i = 0; i < languages.size(); ++i){
            QPair<QString,QLocale> p = convertToLocale(languages[i]);
            QString lang = QLocale::languageToString(p.second.language());
            if( l == lang){
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
        const std::string defLanguage = "System";
        QString applyLanguage(QString lang)
        {
            static QTranslator tr;
            if(tr.isEmpty()){
                qApp->installTranslator(&tr);
            }

            if(lang == common::utils_qt::toQString(defLanguage)){
                lang =  QLocale::languageToString(QLocale::system().language());
            }

            QString qmPath = pathToQm(lang);
            bool isLoad = tr.load(qmPath,trPath());

            if(!isLoad && lang != builtInLanguage){
                return builtInLanguage;
            }

            return lang;
        }

        QStringList getSupportedLanguages()
        {
            const QStringList languages = qmLanguages();

            QStringList result;
            result << common::utils_qt::toQString(defLanguage) << builtInLanguage;
            for(int i = 0; i < languages.size(); ++i){
                QPair<QString,QLocale> p = convertToLocale(languages[i]);
                QString lang = QLocale::languageToString(p.second.language());
                result.append(lang);
            }
            return result;
        }
    }
}
