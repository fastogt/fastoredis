#include "gui/dialogs/history_server_dialog.h"

#include <QLabel>
#include <QHBoxLayout>

#include "gui/base/graph_widget.h"
#include "gui/gui_factory.h"
#include "gui/glass_widget.h"

namespace fastoredis
{
    ServerHistoryDialog::ServerHistoryDialog(const QString &title, connectionTypes type, QWidget *parent)
        : QDialog(parent), type_(type)
    {
        setWindowTitle(title);

        graphWidget_ = new GraphWidget(GraphWidget::nodes_container_type(), this);

        QHBoxLayout *mainL = new QHBoxLayout(this);
        mainL->addWidget(graphWidget_);
        setLayout(mainL);

        glassWidget_ = new GlassWidget(GuiFactory::instance().loadingPathFilePath(), "Loading...", 0.5, QColor(111, 111, 100), this);
    }

    void ServerHistoryDialog::startLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryRequest &req)
    {
        glassWidget_->start();
    }

    void ServerHistoryDialog::finishLoadServerHistoryInfo(const EventsInfo::ServerInfoHistoryResponce &res)
    {
        glassWidget_->stop();
        common::ErrorValue er = res.errorInfo();
        if(!er.isError()){
            if(type_ == REDIS){
                //infos_ = res.infos_;
            }
        }
    }

    void ServerHistoryDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
