#include "core/SettingsManager.h"

#include "common/settings/settings.hpp"
#define INI_PATH ('~','/','.','c','o','n','f','i','g','/','a','l','t','o','/','a','l','t','o','r','e','d','i','s','.','i','n','i')
#define style ('s','t','y','l','e')
#define style2 ('s','t','y','l','e','2')

namespace
{
    typedef common::storages::ini::ini_storage<GEN_STRING_TYPLE(INI_PATH)> static_path_storage;
    BEGIN_DECL_TYPLE(style,unicode_string,static_path_storage)
    BEGIN_DECL_TYPLE(style2,unicode_string,static_path_storage)

    typedef common::storages::storage_container<genereted_settings::setting_style, genereted_settings::setting_style2> static_storage_type;

    typedef common::storages::settings_container<static_storage_type> server_main_t;

    inline server_main_t &get_config_storage()
    {
        static server_main_t g_m(static_storage_type(UTEXT("Native"),UTEXT("~")));
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
}
