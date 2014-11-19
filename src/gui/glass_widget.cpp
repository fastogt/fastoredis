#include "gui/glass_widget.h"

#include <QLabel>
#include <QMovie>
#include <QEvent>

#include <QGraphicsOpacityEffect>

namespace fastoredis
{
    GlassWidget::GlassWidget(const QString& path, const QString& text, qreal opacity, const QColor& color, QObject *parent)
        : QObject(parent), movie_(0), glassColor_(color), text_(text)
    {
        wGlass_ = new QLabel;

        wGlass_->setStyleSheet(QString("background-color: %1;").arg(glassColor_.name()));
        QGraphicsOpacityEffect * tmpEffect = new QGraphicsOpacityEffect;
        tmpEffect->setOpacity(opacity);
        wGlass_->setGraphicsEffect(tmpEffect);

        wAnimationContainer_ = new QLabel;
        wInfoTextContaiter_ = new QLabel;

        movie_ = new QMovie(path, QByteArray(), this);
    }

    GlassWidget::~GlassWidget()
    {
        wGlass_->deleteLater();
        wAnimationContainer_->deleteLater();
        wInfoTextContaiter_->deleteLater();
    }

    void GlassWidget::start()
    {
        QWidget* widget = qobject_cast<QWidget*>(parent());

        wAnimationContainer_->show();
        wGlass_->setParent(widget);
        widget->installEventFilter(this);
        wGlass_->show();

        showInfoTextBlock();
        showAnimationBlock();

        QEvent event(QEvent::Resize);
        eventFilter(0, &event);
    }

    void GlassWidget::stop()
    {
        wGlass_->parentWidget()->removeEventFilter(this);
        wGlass_->hide();
        wGlass_->setParent(0);
        wAnimationContainer_->hide();
        wInfoTextContaiter_->hide();
        wAnimationContainer_->setParent(0);
        wInfoTextContaiter_->setParent(0);
    }

    bool GlassWidget::eventFilter(QObject* /* object */, QEvent* event)
    {
        if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize)){
            wGlass_->resize(wGlass_->parentWidget()->size());
            wGlass_->move(0, 0);
            infoBlockPositioning();
            return true;
        }

        if (event->type() ==  QEvent::Shortcut || event->type() ==  QEvent::ShortcutOverride){

        }
        wGlass_->setFocus();
        event->accept();
        return false;
    }

    void GlassWidget::infoBlockPositioning()
    {
        if (wAnimationContainer_->isVisible() && wInfoTextContaiter_->isVisible()) {
            wAnimationContainer_->move((wGlass_->width() - wAnimationContainer_->width()) / 2,
                wGlass_->height() / 2 - wAnimationContainer_->height());
            wInfoTextContaiter_->move((wGlass_->width() - wInfoTextContaiter_->width()) / 2,
                wGlass_->height() / 2 + wInfoTextContaiter_->height());
        }
        else {
            if (wAnimationContainer_->isVisible()){
                wAnimationContainer_->move((wGlass_->width() - wAnimationContainer_->width()) / 2,
                (wGlass_->height() - wAnimationContainer_->height()) / 2);
            }

            if (wInfoTextContaiter_->isVisible()){
                wInfoTextContaiter_->move((wGlass_->width() - wInfoTextContaiter_->width()) / 2,
                (wGlass_->height() - wInfoTextContaiter_->height()) / 2);
            }
        }
    }

    void GlassWidget::showInfoTextBlock()
    {
        wInfoTextContaiter_->setParent(wGlass_->parentWidget());
        wInfoTextContaiter_->setText(text_);

        wInfoTextContaiter_->show();
        wInfoTextContaiter_->adjustSize();
        infoBlockPositioning();
    }

    void GlassWidget::showAnimationBlock()
    {
        wAnimationContainer_->setParent(wGlass_->parentWidget());
        wAnimationContainer_->setMovie(movie_);
        movie_->jumpToFrame(0);
        wAnimationContainer_->resize(movie_->currentPixmap().size());

        wAnimationContainer_->show();
        movie_->start();

        wAnimationContainer_->show();
        infoBlockPositioning();
    }
}
