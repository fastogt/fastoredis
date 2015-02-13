#include "gui/icon_label.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>

namespace fastoredis
{
    IconLabel::IconLabel(const QIcon& icon, const QString& text, const QSize& size, QWidget* parent)
        : QWidget(parent)
    {
        QHBoxLayout *mainL = new QHBoxLayout;
        icon_ = new QLabel;
        text_ = new QLabel;
        setText(text);
        setIcon(icon, size);

        mainL->addWidget(icon_);
        mainL->addWidget(text_);
        setLayout(mainL);
    }

    void IconLabel::setText(const QString &text)
    {
        text_->setText(text);
    }

    void IconLabel::setIcon(const QIcon &icon, const QSize& size)
    {
        const QPixmap pm = icon.pixmap(size);
        icon_->setPixmap(pm);
    }
}
