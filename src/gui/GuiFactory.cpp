#include "gui/GuiFactory.h"

#include <QApplication>
#include <QStyle>

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
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/redis.png");
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

    const QIcon &GuiFactory::stopIcon() const
    {
        static QIcon stop(":"PROJECT_NAME_LOWERCASE"/icons/stop.png");
        return stop;
    }
}
