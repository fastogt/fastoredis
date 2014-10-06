#include "gui/app_style.h"

#include <QApplication>
#include <QStyleFactory>

#include "common/qt/convert_string.h"

namespace fastoredis
{
    const common::string16 AppStyle::defStyle = UTEXT("Native");

    void applyStyle(const QString &styleName)
    {
        if (styleName == common::convertFromString16<QString>(AppStyle::defStyle)) {
            QApplication::setStyle(new AppStyle);
        }
        else {
            QApplication::setStyle(QStyleFactory::create(styleName));
        }
    }

    QStringList getSupportedStyles()
    {
        static QStringList result = QStringList() << common::convertFromString16<QString>(AppStyle::defStyle) << QStyleFactory::keys();
        return result;
    }

    void AppStyle::drawControl(ControlElement element,	const QStyleOption * option,	QPainter * painter,	const QWidget * widget) const
    {
        return OsStyle::drawControl(element, option, painter, widget);
    }

    void AppStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
#ifdef Q_OS_WIN

        if(element==QStyle::PE_FrameFocusRect)
            return;

#endif // Q_OS_WIN

        return OsStyle::drawPrimitive(element, option, painter, widget);
    }

    QRect AppStyle::subElementRect( SubElement element, const QStyleOption * option, const QWidget * widget /*= 0 */ ) const
    {
        return OsStyle::subElementRect(element, option, widget);
    }
}
