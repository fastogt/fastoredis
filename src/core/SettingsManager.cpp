#include "core/SettingsManager.h"

#include "common/settings/settings.hpp"

#define INI_PATH ('~','/','.','c','o','n','f','i','g','/','f','a','s','t','o','r','e','d','i','s','.','i','n','i')
#define style ('s','t','y','l','e')
#define connections_ ('c','o','n','n','e','c','t','i','o','n','s')

namespace std
{
    inline ostream& operator<< (ostream& stream, const fastoredis::SettingsManager::ConnectionSettingsContainerType& info)
    {
        typedef fastoredis::SettingsManager::ConnectionSettingsContainerType cont_t;
        for(cont_t::const_iterator it = info.begin(); it != info.end(); ++it){
            stream << fastoredis::details::toStdString((*it).get());
        }
        return stream;
    }

    inline istream& operator>> (istream& stream, const fastoredis::SettingsManager::ConnectionSettingsContainerType& info)
    {
        return stream;
    }
}

namespace
{
    typedef common::storages::ini::ini_storage<GEN_STRING_TYPLE(INI_PATH)> static_path_storage;

    BEGIN_DECL_TYPLE(style,unicode_string,static_path_storage)
    BEGIN_DECL_TYPLE(connections_,fastoredis::SettingsManager::ConnectionSettingsContainerType,static_path_storage)

    typedef common::storages::storage_container<genereted_settings::setting_style, genereted_settings::setting_connections_> static_storage_type;

    typedef common::storages::settings_container<static_storage_type> server_main_t;

    inline server_main_t &get_config_storage()
    {
        static server_main_t g_m(static_storage_type(UTEXT("Native"),fastoredis::SettingsManager::ConnectionSettingsContainerType() ));
        return g_m;
    }
}

#define GET_SETTING(NAME) get_config_storage().get_value<NAME>()

namespace fastoredis
{
    SettingsManager::SettingsManager()
    {
    }

    std::string SettingsManager::currentStyle() const
    {
        return GET_SETTING(genereted_settings::setting_style).value();
    }

    void SettingsManager::setCurrentStyle(const std::string &st) const
    {
        GET_SETTING(genereted_settings::setting_style).set_value(st);
    }

    void SettingsManager::addConnection(const IConnectionSettingsBasePtr &con)
    {
        if(con){
            SettingsManager::ConnectionSettingsContainerType conCont = GET_SETTING(genereted_settings::setting_connections_).value();
            conCont.push_back(con);
            GET_SETTING(genereted_settings::setting_connections_).set_value(conCont);
        }
    }

    SettingsManager::ConnectionSettingsContainerType SettingsManager::connections() const
    {
        return GET_SETTING(genereted_settings::setting_connections_).value();
    }
}
