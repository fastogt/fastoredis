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
        static QIcon main(":"PROJECT_NAME_LOWERCASE"/icons/redis.png");
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

    const QIcon &GuiFactory::messageBoxInformationIcon() const
    {
        static QIcon open = qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation);
        return open;
    }

}
