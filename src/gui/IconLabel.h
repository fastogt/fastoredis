#pragma once

#include <QFrame>
QT_BEGIN_NAMESPACE
class QWidget;
class QLabel;
QT_END_NAMESPACE

namespace fastoredis
{
    class IconLabel
            : public QWidget
    {
        Q_OBJECT
    public:
        typedef QWidget base_class;
        IconLabel(const QIcon &icon, const QString &text, QWidget* parent = 0);

        void setText(const QString &text);
        void setIcon(const QIcon &icon);

    private:
        QLabel *_icon;
        QLabel *_text;
    };
}
