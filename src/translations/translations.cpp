#include "translations/Translations.h"

#include <QTranslator>
#include <QApplication>
#include <QDir>

#include "common/qt/converter_patterns.h"

namespace
{
    QDir trPath()
    {
        return QDir(QApplication::applicationDirPath() + "/translations");
    }

    QStringList qmLanguages()
    {
        return trPath().entryList(QStringList(PROJECT_NAME"_*.qm"));
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
    QTranslator *prev_tr = NULL;
}

namespace fastoredis
{
    namespace translations
    {
        const std::string defLanguage = "English";
        namespace detail
        {
            bool applyLanguage(const QString &lang)
            {
                QString qmPath = pathToQm(lang);
                if(!qmPath.isEmpty()){
                    QTranslator *tr = new QTranslator;
                    if(prev_tr){
                        qApp->removeTranslator(prev_tr);
                        delete prev_tr;
                    }
                    prev_tr = tr;

                    if(tr->load(qmPath,trPath().path())){
                        qApp->installTranslator(tr);
                        return true;
                    }
                }
                return false;
            }

            QStringList getSupportedLanguages()
            {
                static const QString defL = common::utils_qt::toQString(defLanguage);
                const QStringList languages = qmLanguages();
                QStringList result;

                result << defL;
                for(int i = 0; i < languages.size(); ++i){
                    QPair<QString,QLocale> p = convertToLocale(languages[i]);
                    QString lang = QLocale::languageToString(p.second.language());
                    if( defL != lang){
                        result.append(lang);
                    }
                }
                return result;
            }
        }
    }
}
