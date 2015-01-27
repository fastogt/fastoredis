#pragma  once

#include <QDialog>

namespace fastoredis
{
    class EncodeDecodeDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        explicit EncodeDecodeDialog(QWidget* parent);

        enum
        {
            height = 480,
            width = 640
        };
    };
}
