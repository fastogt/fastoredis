#include "gui/dialogs/InfoServerDialog.h"

#include <QLabel>
#include <QVBoxLayout>

#include "common/qt/converter_patterns.h"

namespace
{
    const QString versionTemplate = QObject::tr("%1 version: ");
}

namespace fastoredis
{
    InfoServerDialog::InfoServerDialog(connectionTypes type, QWidget *parent)
        : QDialog(parent), databaseName_(common::utils_qt::toQString(toStdString(type)))
    {
        version_ = new QLabel;
        QVBoxLayout *mainL = new QVBoxLayout(this);
        mainL->addWidget(version_);
        setLayout(mainL);
    }

    void InfoServerDialog::startServerInfo(const EventsInfo::ServerInfoRequest &req)
    {

    }

    void InfoServerDialog::finishServerInfo(const EventsInfo::ServerInfoResponce &res)
    {
        using namespace common;
        error::ErrorInfo er = res.errorInfo();
        if(!er.isError()){
            if(utils_qt::toQString(toStdString(REDIS)) == databaseName_){
                ServerInfo::Server ser = res.info_.server_;
                version_->setText(versionTemplate.arg(databaseName_) + utils_qt::toQString(ser.redis_version_));
            }
        }
    }

    void InfoServerDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
