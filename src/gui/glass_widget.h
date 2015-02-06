#pragma once

#include <QWidget>

class QLabel;
class QMovie;

namespace fastoredis
{
    class GlassWidget
            : public QObject
    {
        Q_OBJECT
    public:
        GlassWidget(const QString& path, const QString& text, qreal opacity = 0.5, const QColor& color = QColor(111, 111, 100), QObject* parent = 0);
        ~GlassWidget();

        void start();
        void stop();

    protected:
        virtual bool eventFilter(QObject* object, QEvent* event);

    private:
        void showInfoTextBlock();
        void showAnimationBlock();
        void infoBlockPositioning();

        QLabel* wGlass_;
        QLabel* wAnimationContainer_;
        QLabel* wInfoTextContaiter_;
        QMovie* movie_;
        const QString text_;
    };
}
