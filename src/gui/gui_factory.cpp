#include "gui/gui_factory.h"

#include <QApplication>
#include <QStyle>

namespace fastoredis
{
    const QIcon &GuiFactory::openIcon() const
    {
        static QIcon open(":" PROJECT_NAME_LOWERCASE "/icons/open.png");
        return open;
    }

    const QIcon &GuiFactory::mainWindowIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/logo.png");
        return main;
    }

    const QIcon &GuiFactory::redisConnectionIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/redis.png");
        return main;
    }

    const QIcon& GuiFactory::memcachedConnectionIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/memcached.png");
        return main;
    }

    const QIcon &GuiFactory::connectIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/connect.png");
        return main;
    }

    const QIcon &GuiFactory::disConnectIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/disconnect.png");
        return main;
    }

    const QIcon &GuiFactory::serverIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/server.png");
        return main;
    }

    const QIcon &GuiFactory::addIcon() const
    {
        static QIcon open(":" PROJECT_NAME_LOWERCASE "/icons/add.png");
        return open;
    }

    const QIcon &GuiFactory::removeIcon() const
    {
        static QIcon open(":" PROJECT_NAME_LOWERCASE "/icons/remove.png");
        return open;
    }

    const QIcon &GuiFactory::editIcon() const
    {
        static QIcon open(":" PROJECT_NAME_LOWERCASE "/icons/edit.png");
        return open;
    }

    const QIcon &GuiFactory::messageBoxInformationIcon() const
    {
        static QIcon open = qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation);
        return open;
    }

    const QIcon &GuiFactory::messageBoxQuestionIcon() const
    {
        static QIcon open = qApp->style()->standardIcon(QStyle::SP_MessageBoxQuestion);
        return open;
    }

    const QIcon &GuiFactory::executeIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/start.png");
        return start;
    }

    const QIcon &GuiFactory::timeIcon() const
    {
        static QIcon time(":" PROJECT_NAME_LOWERCASE "/icons/time.png");
        return time;
    }

    const QIcon &GuiFactory::stopIcon() const
    {
        static QIcon stop(":" PROJECT_NAME_LOWERCASE "/icons/stop.png");
        return stop;
    }

    const QIcon &GuiFactory::databaseIcon() const
    {
        static QIcon db(":" PROJECT_NAME_LOWERCASE "/icons/database.png");
        return db;
    }

    const QIcon& GuiFactory::keyIcon() const
    {
        static QIcon db(":" PROJECT_NAME_LOWERCASE "/icons/key.png");
        return db;
    }

    QFont GuiFactory::font() const
    {
#if defined(OS_MACOSX)
        static const QFont textFont = QFont("Monaco",12);
#elif defined(OS_LINUX)
        static QFont textFont = QFont("Monospace");
        textFont.setFixedPitch(true);
#elif defined(OS_WIN)
        static const QFont textFont = QFont("Courier",10);
#elif defined(OS_ANDROID)
        static const QFont textFont = QFont("Monospace");
#endif
        return textFont;
    }

    const QIcon &GuiFactory::icon(connectionTypes type) const
    {
        if(type == REDIS){
            return redisConnectionIcon();
        }
        else if(type == MEMCACHED){
            return memcachedConnectionIcon();
        }
        else{
            return serverIcon();
        }
    }

    const QIcon& GuiFactory::modeIcon(ConnectionMode mode) const
    {
        if(mode == LatencyMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/latency_mode.png");
            return i;
        }
        else if(mode == SlaveMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/slave_mode.png");
            return i;
        }
        else if(mode == GetRDBMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/rdb_mode.png");
            return i;
        }
        else if(mode == PipeMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/pipe_mode.png");
            return i;
        }
        else if(mode == FindBigKeysMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/bigkeys_mode.png");
            return i;
        }
        else if(mode == StatMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/stat_mode.png");
            return i;
        }
        else if(mode == ScanMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/scan_mode.png");
            return i;
        }
        else if(mode == IntaractiveMode){
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/interactive_mode.png");
            return i;
        }
        else{
            static QIcon err(":" PROJECT_NAME_LOWERCASE "/icons/error.png");
            NOTREACHED();
            return err;
        }
    }

    const QIcon &GuiFactory::icon(common::Value::Type type) const
    {
        switch(type)
        {
        case common::Value::TYPE_NULL:
            static QIcon u(":" PROJECT_NAME_LOWERCASE "/icons/unknown.png");
            return u;
        case common::Value::TYPE_BOOLEAN:
            static QIcon b(":" PROJECT_NAME_LOWERCASE "/icons/integer.png");
            return b;
        case common::Value::TYPE_STRING:
            static QIcon s(":" PROJECT_NAME_LOWERCASE "/icons/string.png");
            return s;
        case common::Value::TYPE_ARRAY:
            static QIcon a(":" PROJECT_NAME_LOWERCASE "/icons/array.png");
            return a;
        case common::Value::TYPE_INTEGER:
        case common::Value::TYPE_DOUBLE:
        case common::Value::TYPE_UINTEGER:
            static QIcon i(":" PROJECT_NAME_LOWERCASE "/icons/integer.png");
            return i;
        case common::Value::TYPE_STATUS:
            static QIcon st(":" PROJECT_NAME_LOWERCASE "/icons/status.png");
            return st;
        case common::Value::TYPE_ERROR:
            static QIcon er(":" PROJECT_NAME_LOWERCASE "/icons/error.png");
            return er;
        default:
            static QIcon err(":" PROJECT_NAME_LOWERCASE "/icons/error.png");
            NOTREACHED();
            return err;
        }
    }

    const QIcon &GuiFactory::loadIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/load.png");
        return start;
    }

    const QIcon &GuiFactory::saveIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/save.png");
        return start;
    }

    const QIcon &GuiFactory::saveAsIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/saveas.png");
        return start;
    }

    const QIcon &GuiFactory::textIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/text.png");
        return start;
    }

    const QIcon &GuiFactory::tableIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/table.png");
        return start;
    }

    const QIcon &GuiFactory::treeIcon() const
    {
        static QIcon start(":" PROJECT_NAME_LOWERCASE "/icons/tree.png");
        return start;
    }

    const QString &GuiFactory::pathToLoadingGif() const
    {
        static QString path(":" PROJECT_NAME_LOWERCASE "/icons/loading.gif");
        return path;
    }

    const QIcon &GuiFactory::loggingIcon() const
    {
        static QIcon logg(":" PROJECT_NAME_LOWERCASE "/icons/logging.png");
        return logg;
    }

    const QIcon &GuiFactory::commandIcon() const
    {
        static QIcon comm(":" PROJECT_NAME_LOWERCASE "/icons/command.png");
        return comm;
    }

    const QIcon& GuiFactory::preferencesIcon() const
    {
        static QIcon pref(":" PROJECT_NAME_LOWERCASE "/icons/preferences.png");
        return pref;
    }

    const QIcon& GuiFactory::commandIcon(connectionTypes type) const
    {
        if(type == REDIS){
            return redisConnectionIcon();
        }
        else if(type == MEMCACHED){
            return memcachedConnectionIcon();
        }
        else{
            return serverIcon();
        }
    }

    const QIcon& GuiFactory::pythonIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/python.png");
        return main;
    }

    const QIcon& GuiFactory::luaIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/lua.png");
        return main;
    }

    const QIcon& GuiFactory::encodeDecodeIcon() const
    {
        static QIcon main(":" PROJECT_NAME_LOWERCASE "/icons/encode_decode.png");
        return main;
    }

    const QIcon& GuiFactory::close16Icon() const
    {
        static QIcon close(":" PROJECT_NAME_LOWERCASE "/icons/close16.png");
        return close;
    }

    const QIcon& GuiFactory::typeIcon(connectionTypes type) const
    {
        if(type == REDIS){
            static QIcon comm(":" PROJECT_NAME_LOWERCASE "/icons/redistype.png");
            return comm;
        }
        else{
            static QIcon comm(":" PROJECT_NAME_LOWERCASE "/icons/redistype.png");
            return comm;
        }
    }
}
