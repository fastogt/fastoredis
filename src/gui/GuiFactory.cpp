#include "gui/GuiFactory.h"

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
}
