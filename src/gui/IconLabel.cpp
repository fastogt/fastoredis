#include "gui/IconLabel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>

namespace fastoredis
{
    IconLabel::IconLabel(const QIcon &icon, const QString &text, QWidget* parent)
        : base_class(parent)
    {
        QHBoxLayout *mainL = new QHBoxLayout;
        _icon = new QLabel;
        _text = new QLabel;
        setText(text);
        setIcon(icon);

        mainL->addWidget(_icon);
        mainL->addWidget(_text);
        setLayout(mainL);
    }

    void IconLabel::setText(const QString &text)
    {
        _text->setText(text);
    }

    void IconLabel::setIcon(const QIcon &icon)
    {
        _icon->setPixmap(icon.pixmap(16, 16));
    }
}
