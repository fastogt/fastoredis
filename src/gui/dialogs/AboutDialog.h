#pragma  once

#include <QDialog>
#include "global/global.h"

namespace fastoredis
{
    class AboutDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AboutDialog(QWidget *parent);
    };
}
