#include "gui/dialogs/property_server_dialog.h"

#include <QLabel>
#include <QHBoxLayout>

#include "common/qt/convert_string.h"
#include "gui/gui_factory.h"
#include "gui/glass_widget.h"
#include "gui/property_table_model.h"

namespace fastoredis
{
    PropertyServerDialog::PropertyServerDialog(const QString &title, connectionTypes type, QWidget *parent)
        : QDialog(parent), type_(type)
    {
        setWindowTitle(title);
        propertyes_table_ = new QTableView(this);
        PropertyTableModel *mod = new PropertyTableModel(propertyes_table_);
        VERIFY(connect(mod, SIGNAL(changedProperty(const PropertyType &)), this, SIGNAL(changedProperty(const PropertyType &))));
        propertyes_table_->setModel(mod);

        QHBoxLayout *mainL = new QHBoxLayout(this);
        mainL->addWidget(propertyes_table_);
        setLayout(mainL);

        glassWidget_ = new GlassWidget(GuiFactory::instance().loadingPathFilePath(), "Loading...", 0.5, QColor(111, 111, 100), this);
    }

    void PropertyServerDialog::startServerProperty(const EventsInfo::ServerPropertyInfoRequest &req)
    {
        glassWidget_->start();
    }

    void PropertyServerDialog::finishServerProperty(const EventsInfo::ServerPropertyInfoResponce &res)
    {
        glassWidget_->stop();
        common::ErrorValue er = res.errorInfo();
        if(!er.isError()){
            if(type_ == REDIS){
                ServerPropertyInfo inf = res.info_;
                PropertyTableModel *model = qobject_cast<PropertyTableModel*>(propertyes_table_->model());
                for(int i = 0; i < inf.propertyes_.size(); ++i)
                {
                    PropertyType it = inf.propertyes_[i];
                    model->addItem(new PropertyTableItem(common::convertfromString<QString>(it.first), common::convertfromString<QString>(it.second)));
                }
            }
        }
    }

    void PropertyServerDialog::startServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoRequest &req)
    {

    }

    void PropertyServerDialog::finishServerChangeProperty(const EventsInfo::ChangeServerPropertyInfoResponce &res)
    {
        common::ErrorValue er = res.errorInfo();
        if(!er.isError()){
            if(type_ == REDIS){
                PropertyType pr = res.newItem_;
                if(res.isChange_){
                    PropertyTableModel *model = qobject_cast<PropertyTableModel*>(propertyes_table_->model());
                    model->changeProperty(pr);
                }
            }
        }
    }

    void PropertyServerDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
