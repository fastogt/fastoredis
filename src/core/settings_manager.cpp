#include "core/settings_manager.h"

#include <QSettings>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>

#include "translations/translations.h"
#include "gui/app_style.h"

#include "common/file_system.h"
#include "common/qt/convert_string.h"

#ifdef OS_WIN
#define PYTHON_EXE "python.exe"
#else
#define PYTHON_EXE "python"
#endif

#define PREFIX "settings/"

#define LANGUAGE PREFIX"language"
#define STYLE PREFIX"style"
#define CONNECTIONS PREFIX"connections"
#define VIEW PREFIX"view"
#define SYNCTABS PREFIX"synctabs"
#define LOGGINGDIR PREFIX"loggingdir"
#define CHECKUPDATES PREFIX"checkupdates"
#define PYTHONEXECPATH PREFIX"pythonexecpath"

namespace
{
    const std::string iniPath("~/.config/"PROJECT_NAME"/config.ini");

    std::string pythonExecPath()
    {
        std::string path;
        bool res = common::file_system::findFileInPath(PYTHON_EXE, path);
        if(res){
            return common::file_system::prepare_path(path);
        }

        return std::string();
    }

    std::string encode_base64(const std::string& val)
    {
        using namespace boost::archive::iterators;
        typedef transform_width< binary_from_base64<remove_whitespace<std::string::const_iterator> >,8,6 > binary_text;
        std::string enc( binary_text(val.begin()), binary_text(val.end()));
        return enc;
    }

    std::string decode_base64(const std::string& val)
    {
        using namespace boost::archive::iterators;
        typedef insert_linebreaks<base64_from_binary<transform_width<std::string::const_iterator,6,8> >, 512 > base64_text;
        std::string dec( base64_text(val.begin()), base64_text(val.end()));
        return dec;
    }
}


namespace fastoredis
{
    SettingsManager::SettingsManager()
        : views_(), curStyle_(), curLanguage_(), connections_(), syncTabs_(), loggingDir_(), autoCheckUpdate_(), pythonExecPath_()
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
            std::string raw = encode_base64(encoded);

            IConnectionSettingsBaseSPtr sett(IConnectionSettingsBase::fromString(raw));
            if(sett){
               connections_.push_back(sett);
            }
        }

        syncTabs_= settings.value(SYNCTABS, false).toBool();

        std::string dir = common::file_system::get_dir_path(iniPath);
        loggingDir_ = settings.value(LOGGINGDIR, common::convertFromString<QString>(dir)).toString();
        autoCheckUpdate_ = settings.value(CHECKUPDATES, true).toBool();
        pythonExecPath_ = settings.value(PYTHONEXECPATH, common::convertFromString<QString>(::pythonExecPath())).toString();
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
               std::string decoded = decode_base64(raw);
               QString qdata = common::convertFromString<QString>(decoded);
               connections.push_back(qdata);
            }
        }
        settings.setValue(CONNECTIONS, connections);

        settings.setValue(SYNCTABS, syncTabs_);
        settings.setValue(LOGGINGDIR, loggingDir_);
        settings.setValue(CHECKUPDATES, autoCheckUpdate_);
        settings.setValue(PYTHONEXECPATH, pythonExecPath_);
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

    QString SettingsManager::pythonExecPath() const
    {
        return pythonExecPath_;
    }

    void SettingsManager::setPythonExecPath(const QString &path)
    {
        pythonExecPath_ = path;
    }
}
