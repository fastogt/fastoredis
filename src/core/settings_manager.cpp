#include "core/settings_manager.h"

#include <QSettings>

#include "translations/translations.h"

#include "gui/app_style.h"

#include "common/file_system.h"
#include "common/qt/convert_string.h"
#include "common/utils.h"

#define PREFIX "settings/"

#define LANGUAGE PREFIX"language"
#define STYLE PREFIX"style"
#define CONNECTIONS PREFIX"connections"
#define VIEW PREFIX"view"
#define SYNCTABS PREFIX"synctabs"
#define LOGGINGDIR PREFIX"loggingdir"
#define CHECKUPDATES PREFIX"checkupdates"

namespace
{
    const std::string iniPath("~/.config/" PROJECT_NAME "/config.ini");
}


namespace fastoredis
{
    SettingsManager::SettingsManager()
        : views_(), curStyle_(), curLanguage_(), connections_(), syncTabs_(), loggingDir_(), autoCheckUpdate_()
    {
       load();
    }

    void SettingsManager::load()
    {
        QString inip = common::convertFromString<QString>(common::file_system::prepare_path(iniPath));
        QSettings settings(inip, QSettings::IniFormat);
        DCHECK(settings.status() == QSettings::NoError);

        curStyle_ = settings.value(STYLE, fastoredis::defStyle).toString();
        curLanguage_ = settings.value(LANGUAGE, fastoredis::translations::defLanguage).toString();

        int view = settings.value(VIEW, fastoredis::Tree).toInt();
        views_ = static_cast<supportedViews>(view);

        QList<QVariant> connections = settings.value(CONNECTIONS, "").toList();
        for(QList<QVariant>::const_iterator it = connections.begin(); it != connections.end(); ++it){
            QVariant var = *it;
            QString string = var.toString();
            std::string encoded = common::convertToString(string);
            std::string raw = common::utils::base64::decode64(encoded);

            IConnectionSettingsBaseSPtr sett(IConnectionSettingsBase::fromString(raw));
            if(sett){
               connections_.push_back(sett);
            }
        }

        syncTabs_= settings.value(SYNCTABS, false).toBool();
        std::string dir = common::file_system::get_dir_path(iniPath);
        loggingDir_ = settings.value(LOGGINGDIR, common::convertFromString<QString>(dir)).toString();
        autoCheckUpdate_ = settings.value(CHECKUPDATES, true).toBool();
    }

    void SettingsManager::save()
    {
        QSettings settings(common::convertFromString<QString>(common::file_system::prepare_path(iniPath)), QSettings::IniFormat);
        DCHECK(settings.status() == QSettings::NoError);

        settings.setValue(STYLE, curStyle_);
        settings.setValue(LANGUAGE, curLanguage_);
        settings.setValue(VIEW, views_);

        QList<QVariant> connections;
        for(ConnectionSettingsContainerType::const_iterator it = connections_.begin(); it != connections_.end(); ++it){
            IConnectionSettingsBaseSPtr conn = *it;
            if(conn){
               std::string raw = conn->toString();
               std::string enc = common::utils::base64::encode64(raw);
               QString qdata = common::convertFromString<QString>(enc);
               connections.push_back(qdata);
            }
        }
        settings.setValue(CONNECTIONS, connections);

        settings.setValue(SYNCTABS, syncTabs_);
        settings.setValue(LOGGINGDIR, loggingDir_);
        settings.setValue(CHECKUPDATES, autoCheckUpdate_);
    }

    SettingsManager::~SettingsManager()
    {
        save();
    }

    QString SettingsManager::currentStyle() const
    {
        return curStyle_;
    }

    void SettingsManager::setCurrentStyle(const QString &st)
    {
        curStyle_ = st;
    }

    QString SettingsManager::currentLanguage() const
    {
        return curLanguage_;
    }

    void SettingsManager::setCurrentLanguage(const QString &lang)
    {
        curLanguage_ = lang;
    }

    supportedViews SettingsManager::defaultView() const
    {
        return views_;
    }

    void SettingsManager::setDefaultView(supportedViews view)
    {
        views_ = view;
    }

    void SettingsManager::addConnection(const IConnectionSettingsBaseSPtr &connection)
    {
        if(connection){
            ConnectionSettingsContainerType::iterator it = std::find(connections_.begin(),connections_.end(),connection);
            if (it == connections_.end()) {
                connections_.push_back(connection);
            }
        }
    }

    void SettingsManager::removeConnection(const IConnectionSettingsBaseSPtr &connection)
    {
        if(connection){
            ConnectionSettingsContainerType::iterator it = std::find(connections_.begin(),connections_.end(),connection);
            if (it != connections_.end()) {
                connections_.erase(it);
            }
        }
    }

    SettingsManager::ConnectionSettingsContainerType SettingsManager::connections() const
    {
        return connections_;
    }

    bool SettingsManager::syncTabs() const
    {
        return syncTabs_;
    }

    void SettingsManager::setSyncTabs(bool sync)
    {
        syncTabs_ = sync;
    }

    QString SettingsManager::loggingDirectory() const
    {
        return loggingDir_;
    }

    void SettingsManager::setLoggingDirectory(const QString &dir)
    {
        loggingDir_ = dir;
    }

    bool SettingsManager::autoCheckUpdates() const
    {
        return autoCheckUpdate_;
    }

    void SettingsManager::setAutoCheckUpdates(bool isCheck)
    {
        autoCheckUpdate_ = isCheck;
    }
}
