#pragma once

#include <QProxyStyle>

#include "common/types.h"

namespace fastoredis
{
    extern const QString defStyle;

    void applyStyle(const QString& styleName);
    QStringList getSupportedStyles();

    class AppStyle 
        : public QProxyStyle
    {
        Q_OBJECT
    public:        
        virtual void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        virtual QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget = 0) const;
    };
}
