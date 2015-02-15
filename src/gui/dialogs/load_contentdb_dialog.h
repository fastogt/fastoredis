#pragma once

#include <QDialog>

#include "core/connection_types.h"

class QLineEdit;

namespace fastoredis
{
    class LoadContentDbDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit LoadContentDbDialog(const QString& title, connectionTypes type, QWidget* parent = 0);
        uint32_t count() const;
        QString pattern() const;

    public Q_SLOTS:
        virtual void accept();

    private:
        const connectionTypes type_;
        QLineEdit* countTextEdit_;
        QLineEdit* patternEdit_;
    };
}
