#include "gui/dialogs/InfoServerDialog.h"

namespace fastoredis
{
    InfoServerDialog::InfoServerDialog(QWidget *parent)
        : QDialog(parent)
    {
    }

    void InfoServerDialog::startServerInfo(const EventsInfo::ServerInfoRequest &req)
    {

    }

    void InfoServerDialog::finishServerInfo(const EventsInfo::ServerInfoResponce &res)
    {

    }

    void InfoServerDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
