#pragma  once

#include <QDialog>

#include "core/events/events_info.h"

class QLineEdit;

namespace fastoredis
{
    class GlassWidget;

    class ChangePasswordServerDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        enum
        {
            fix_height = 160,
            fix_width = 240
        };

        explicit ChangePasswordServerDialog(const QString& title, IServerSPtr server, QWidget* parent);

    private Q_SLOTS:
        void tryToCreatePassword();
        void startChangePassword(const EventsInfo::ChangePasswordRequest& req);
        void finishChangePassword(const EventsInfo::ChangePasswordResponce& res);

    private:
        bool validateInput();
        GlassWidget *glassWidget_;
        QLineEdit* passwordLineEdit_;
        QLineEdit* confPasswordLineEdit_;
        const IServerSPtr server_;
    };
}
