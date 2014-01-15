#include "gui/dialogs/InfoServerDialog.h"

#include <QLabel>
#include <QVBoxLayout>

#include "common/qt/converter_patterns.h"

namespace
{
    const QString redisTextTemplate = QObject::tr("Server:<br/>"
                                                  "Redis version: %1<br/>"
                                                  "Redis git_sha1: %2<br/>"
                                                  "Redis git_dirty: %3<br/>"
                                                  "Redis mode: %4<br/>"
                                                  "Os: %5<br/>"
                                                  "Arch: %6<br/>"
                                                  "Multiplexing Api: %7<br/>"
                                                  "Gcc version: %8<br/>"
                                                  "Process id: %9<br/>"
                                                  "Run id: %10<br/>"
                                                  "Tcp port: %11<br/>"
                                                  "Uptime sec: %12<br/>"
                                                  "Uptime days: %13<br/>"
                                                  "Lru clock: %14<br/>");
}

namespace fastoredis
{
    InfoServerDialog::InfoServerDialog(connectionTypes type, QWidget *parent)
        : QDialog(parent), type_(type)
    {
        serverTextInfo_ = new QLabel;
        QVBoxLayout *mainL = new QVBoxLayout(this);
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
            if(type_ == REDIS){
                ServerInfo::Server ser = res.info_.server_;
                QString textD = redisTextTemplate.arg(utils_qt::toQString(ser.redis_version_))
                        .arg(utils_qt::toQString(ser.redis_git_sha1_))
                        .arg(utils_qt::toQString(ser.redis_git_dirty_))
                        .arg(utils_qt::toQString(ser.redis_mode_))
                        .arg(utils_qt::toQString(ser.os_))
                        .arg(ser.arch_bits_)
                        .arg(utils_qt::toQString(ser.multiplexing_api_))
                        .arg(utils_qt::toQString(ser.gcc_version_))
                        .arg(ser.process_id_)
                        .arg(utils_qt::toQString(ser.run_id_))
                        .arg(ser.tcp_port_)
                        .arg(ser.uptime_in_seconds_)
                        .arg(ser.uptime_in_days_)
                        .arg(ser.lru_clock_);
                        serverTextInfo_->setText(textD);
            }
        }
    }

    void InfoServerDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
