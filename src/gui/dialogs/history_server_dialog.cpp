#include "gui/dialogs/history_server_dialog.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QSplitter>
#include <QComboBox>

#include "common/qt/convert_string.h"
#include "core/redis/redis_infos.h"

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
        settingsGraph_ = new QWidget;
        QHBoxLayout *mainL = new QHBoxLayout(this);

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);

        mainL->addWidget(splitter);
        splitter->addWidget(settingsGraph_);

        serverInfoGroupsNames_ = new QComboBox;
        serverInfoFields_ = new QComboBox;

        VERIFY(connect(serverInfoGroupsNames_, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshInfoFields(int)) ));

        for(int i = 0; i < SIZEOFMASS(redisHeaders); ++i){
            serverInfoGroupsNames_->addItem(common::convertfromString<QString>(redisHeaders[i]));
        }

        QVBoxLayout *setingsLayout = new QVBoxLayout(this);
        setingsLayout->addWidget(serverInfoGroupsNames_);
        setingsLayout->addWidget(serverInfoFields_);
        settingsGraph_->setLayout(setingsLayout);

        splitter->addWidget(graphWidget_);
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
                infos_ = res.infos_;
            }
        }
    }

    void ServerHistoryDialog::refreshInfoFields(int index)
    {
        serverInfoFields_->clear();
        std::pair<FieldPropertyS,FieldPropertyU> field = redisFields[index];
        const int sizeMass = field.second.size_;
        for(int i = 0; i < sizeMass; ++i){
            const unsigned char indexInner = field.second.mass_[i];
            serverInfoFields_->addItem(common::convertfromString<QString>(field.first.mass_[indexInner]), indexInner);
        }
    }

    void ServerHistoryDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
