#include "core/SettingsManager.h"

#include "common/settings/settings.hpp"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#ifdef OS_WIN
#define INI_PATH ('~','/','f','a','s','t','o','r','e','d','i','s','.','i','n','i')
#else
#define INI_PATH ('~','/','.','c','o','n','f','i','g','/','f','a','s','t','o','r','e','d','i','s','.','i','n','i')
#endif
#define style ('s','t','y','l','e')
#define connections_ ('c','o','n','n','e','c','t','i','o','n','s')

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

class ConnectionTranslator
{
public:
    typedef std::string  internal_type;
    typedef fastoredis::SettingsManager::ConnectionSettingsContainerType external_type;

    ConnectionTranslator(){}
    boost::optional<external_type> get_value( internal_type const &v )
    {
        using namespace boost::archive::iterators;
        typedef transform_width< binary_from_base64<remove_whitespace<std::string::const_iterator> >, 8, 6 > binary_text;
        external_type result;
        internal_type text;
        for(internal_type::const_iterator it = v.begin(); it != v.end(); ++it){
            char ch = *it;
            if(ch == ','){
                std::string enc( binary_text(text.begin()), binary_text(text.end()));
                fastoredis::IConnectionSettingsBasePtr item(fastoredis::IConnectionSettingsBase::fromStdString(enc));
                if(item){
                    result.push_back(item);
                }
                text.clear();
            }
            else{
                text += ch;
            }
        }
        return result;
    }
    boost::optional<internal_type> put_value( external_type const& v )
    {
        using namespace boost::archive::iterators;
        typedef insert_linebreaks<base64_from_binary<transform_width<std::string::const_iterator,6,8> >, 72 > base64_text;
        std::ostringstream stream;
        for(external_type::const_iterator it = v.begin(); it != v.end(); ++it){
            std::string text = (*it)->toString();
            std::copy( base64_text(text.begin()), base64_text(text.end()), std::ostream_iterator<char>(stream) );
            stream << ',';
        }
        return stream.str();
    }
};

namespace boost
{
    namespace property_tree
    {
        template<>
        struct translator_between<std::string, fastoredis::SettingsManager::ConnectionSettingsContainerType>
        {
            typedef ConnectionTranslator type;
        };
    }
}

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

    void SettingsManager::addConnection(const IConnectionSettingsBasePtr &connection){
        if(connection){
            SettingsManager::ConnectionSettingsContainerType conCont = GET_SETTING(genereted_settings::setting_connections_).value();
            ConnectionSettingsContainerType::iterator it = std::find(conCont.begin(),conCont.end(),connection);
            if (it == conCont.end()) {
                conCont.push_back(connection);
            }
            GET_SETTING(genereted_settings::setting_connections_).set_value(conCont);
        }
    }

    void SettingsManager::removeConnection(const IConnectionSettingsBasePtr &connection){
        if(connection){
            SettingsManager::ConnectionSettingsContainerType conCont = GET_SETTING(genereted_settings::setting_connections_).value();
            ConnectionSettingsContainerType::iterator it = std::find(conCont.begin(),conCont.end(),connection);
            if (it != conCont.end()) {
                conCont.erase(it);
            }
            GET_SETTING(genereted_settings::setting_connections_).set_value(conCont);
        }
    }

    SettingsManager::ConnectionSettingsContainerType SettingsManager::connections() const
    {
        return GET_SETTING(genereted_settings::setting_connections_).value();
    }
}
