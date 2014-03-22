#include "gui/gui_factory.h"

#include <QApplication>
#include <QStyle>

#include "common/macros.h"

namespace fastoredis
{
    const QIcon &GuiFactory::openIcon() const
    {
        static QIcon open;
        return open;
    }

    const QIcon &GuiFactory::mainWindowIcon() const
    {
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/logo.png");
        return main;
    }

    const QIcon &GuiFactory::redisConnectionIcon() const
    {
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/redis.png");
        return main;
    }

    const QIcon &GuiFactory::connectIcon() const
    {
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/connect.png");
        return main;
    }

    const QIcon &GuiFactory::disConnectIcon() const
    {
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/disconnect.png");
        return main;
    }

    const QIcon &GuiFactory::serverIcon() const
    {
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/server.png");
        return main;
    }

    const QIcon &GuiFactory::addIcon() const
    {
        static QIcon open(":"PROJECT_NAME_LOWERCASE"/icons/add.png");
        return open;
    }

    const QIcon &GuiFactory::removeIcon() const
    {
        static QIcon open(":"PROJECT_NAME_LOWERCASE"/icons/remove.png");
        return open;
    }

    const QIcon &GuiFactory::editIcon() const
    {
        static QIcon open(":"PROJECT_NAME_LOWERCASE"/icons/edit.png");
        return open;
    }

    const QIcon &GuiFactory::messageBoxInformationIcon() const
    {
        static QIcon open = qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation);
        return open;
    }

    const QIcon &GuiFactory::executeIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/start.png");
        return start;
    }

    const QIcon &GuiFactory::timeIcon() const
    {
        static QIcon time(":"PROJECT_NAME_LOWERCASE"/icons/time.png");
        return time;
    }

    const QIcon &GuiFactory::stopIcon() const
    {
        static QIcon stop(":"PROJECT_NAME_LOWERCASE"/icons/stop.png");
        return stop;
    }

    const QIcon &GuiFactory::databaseIcon() const
    {
        static QIcon db(":"PROJECT_NAME_LOWERCASE"/icons/database.png");
        return db;
    }

    QFont GuiFactory::font() const
    {
#if defined(Q_OS_MAC)
        static const QFont textFont = QFont("Monaco",12);
#elif defined(Q_OS_UNIX)
        static QFont textFont = QFont("Monospace");
        textFont.setFixedPitch(true);
#elif defined(Q_OS_WIN)
        static const QFont textFont = QFont("Courier",10);
#endif
        return textFont;
    }

    const QIcon &GuiFactory::getIcon(connectionTypes type) const
    {
        if(type == REDIS){
            return redisConnectionIcon();
        }
        else{
            return serverIcon();
        }
    }

    const QIcon &GuiFactory::getIcon(common::Value::Type type) const
    {
        switch(type)
        {
        case common::Value::TYPE_NULL:
            static QIcon u(":"PROJECT_NAME_LOWERCASE"/icons/unknown.png");
            return u;
        case common::Value::TYPE_STRING:
            static QIcon s(":"PROJECT_NAME_LOWERCASE"/icons/string.png");
            return s;
        case common::Value::TYPE_ARRAY:
            static QIcon a(":"PROJECT_NAME_LOWERCASE"/icons/array.png");
            return a;
        case common::Value::TYPE_INTEGER:
            static QIcon i(":"PROJECT_NAME_LOWERCASE"/icons/integer.png");
            return i;
        /*case NIL:
            static QIcon n(":"PROJECT_NAME_LOWERCASE"/icons/null.png");
            return n;*/
        case common::Value::TYPE_STATUS:
            static QIcon st(":"PROJECT_NAME_LOWERCASE"/icons/status.png");
            return st;
        case common::Value::TYPE_ERROR:
            static QIcon er(":"PROJECT_NAME_LOWERCASE"/icons/error.png");
            return er;
        default:
            DCHECK(0);
        }
    }

    const QIcon &GuiFactory::loadIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/load.png");
        return start;
    }

    const QIcon &GuiFactory::saveIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/save.png");
        return start;
    }

    const QIcon &GuiFactory::saveAsIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/saveas.png");
        return start;
    }

    const QIcon &GuiFactory::textIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/text.png");
        return start;
    }

    const QIcon &GuiFactory::tableIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/table.png");
        return start;
    }

    const QIcon &GuiFactory::treeIcon() const
    {
        static QIcon start(":"PROJECT_NAME_LOWERCASE"/icons/tree.png");
        return start;
    }

    const QString &GuiFactory::loadingPathFilePath() const
    {
        static QString path(":"PROJECT_NAME_LOWERCASE"/icons/loading.gif");
        return path;
    }

    const QIcon &GuiFactory::loggingIcon() const
    {
        static QIcon logg(":"PROJECT_NAME_LOWERCASE"/icons/logging.png");
        return logg;
    }

    const QIcon &GuiFactory::commandIcon() const
    {
        static QIcon comm(":"PROJECT_NAME_LOWERCASE"/icons/command.png");
        return comm;
    }
}
