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
        : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint ), type_(type)
    {
        setWindowTitle(title);

        graphWidget_ = new GraphWidget(this);
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
        VERIFY(connect(serverInfoFields_, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshGraph(int)) ));
        for(int i = 0; i < SIZEOFMASS(redisHeaders); ++i){
            serverInfoGroupsNames_->addItem(common::convertFromString<QString>(redisHeaders[i]));
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
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        if(type_ == REDIS){
            infos_ = res.infos();
        }
    }

    void ServerHistoryDialog::refreshInfoFields(int index)
    {
        if(index == -1){
            return;
        }

        serverInfoFields_->clear();
        std::pair<FieldPropertyS,FieldPropertyU> field = redisFields[index];
        const int sizeMass = field.second.size_;
        for(int i = 0; i < sizeMass; ++i){
            const unsigned char indexInner = field.second.mass_[i];
            serverInfoFields_->addItem(common::convertFromString<QString>(field.first.mass_[indexInner]), indexInner);
        }
    }

    void ServerHistoryDialog::refreshGraph(int index)
    {
        if(index == -1){
            return;
        }

        unsigned char serverIndex = serverInfoGroupsNames_->currentIndex();
        QVariant var = serverInfoFields_->itemData(index);
        unsigned char indexIn = qvariant_cast<unsigned char>(var);
        GraphWidget::nodes_container_type nodes;
        for(EventsInfo::ServerInfoHistoryResponce::infos_container_type::iterator it = infos_.begin(); it != infos_.end(); ++it)
        {
            EventsInfo::ServerInfoHistoryResponce::infos_container_type::value_type val = *it;
            common::Value* value = val.second->valueByIndexes(serverIndex, indexIn); //allocate
            if(value){
                qreal graphY = 0.0f;
                if(value->getAsDouble(&graphY)){
                    nodes.push_back(std::make_pair(val.first, graphY));
                }
            }
            delete value;
        }

        graphWidget_->setNodes(nodes);
    }

    void ServerHistoryDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
