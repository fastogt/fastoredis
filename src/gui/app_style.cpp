#include "gui/app_style.h"

#include <QApplication>
#include <QStyleFactory>

namespace fastoredis
{
    const QString defStyle = "Native";

    void applyStyle(const QString& styleName)
    {
        if (styleName == defStyle) {
            QApplication::setStyle(new AppStyle);
        }
        else {
            QApplication::setStyle(QStyleFactory::create(styleName));
        }
    }

    QStringList getSupportedStyles()
    {
        static QStringList result = QStringList() << defStyle << QStyleFactory::keys();
        return result;
    }

    void AppStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
    {
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    void AppStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
    {
#ifdef OS_WIN

        if(element==QStyle::PE_FrameFocusRect)
            return;

#endif // Q_OS_WIN

        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    QRect AppStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget /*= 0 */) const
    {
        return QProxyStyle::subElementRect(element, option, widget);
    }
}
