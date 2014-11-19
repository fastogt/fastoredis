#pragma once

/**/

#include <QWidget>

class QLabel;

namespace fastoredis
{
    class IconLabel
            : public QWidget
    {
        Q_OBJECT
    public:
        IconLabel(const QIcon& icon, const QString& text, const QSize& size, QWidget* parent = 0);

        void setText(const QString& text);
        void setIcon(const QIcon& icon, const QSize& size);

    private:
        QLabel* icon_;
        QLabel* text_;
    };
}
