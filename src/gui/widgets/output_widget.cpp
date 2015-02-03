#include "gui/widgets/output_widget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>

#include "gui/fasto_text_view.h"
#include "gui/fasto_table_view.h"
#include "gui/fasto_tree_view.h"
#include "gui/fasto_common_model.h"
#include "gui/fasto_common_item.h"

#include "gui/gui_factory.h"
#include "gui/icon_label.h"

#include "core/settings_manager.h"
#include "core/iserver.h"

#include "common/qt/convert_string.h"
#include "common/utf_string_conversions.h"
#include "common/time.h"

namespace
{
    fastoredis::FastoCommonItem *createItem(fastoredis::TreeItem* parent, fastoredis::FastoObject* item, const QString& key)
    {
        const std::string value = item->toString();
        return new fastoredis::FastoCommonItem(key, common::convertFromString<QString>(value), item->type(), parent, item);
    }
}

namespace fastoredis
{
    OutputWidget::OutputWidget(IServerSPtr server, QWidget* parent)
        : QWidget(parent)
    {
        commonModel_ = new FastoCommonModel(this);
        VERIFY(connect(commonModel_, SIGNAL(changedValue(const DbValue&)), server.get(), SLOT(changeValue(const DbValue&)), Qt::DirectConnection));
        VERIFY(connect(server.get(), SIGNAL(startedChangeDbValue(const EventsInfo::ChangeDbValueRequest&)), this, SLOT(startChangeDbValue(const EventsInfo::ChangeDbValueRequest&)), Qt::DirectConnection));
        VERIFY(connect(server.get(), SIGNAL(finishedChangeDbValue(const EventsInfo::ChangeDbValueResponce&)), this, SLOT(finishChangeDbValue(const EventsInfo::ChangeDbValueResponce&)), Qt::DirectConnection));


        treeView_ = new FastoTreeView;
        treeView_->setModel(commonModel_);

        tableView_ = new FastoTableView;
        tableView_->setModel(commonModel_);

        textView_ = new FastoTextView(server->outputDelemitr());
        textView_->setModel(commonModel_);
        textView_->setReadOnly(true);

        timeLabel_ = new IconLabel(GuiFactory::instance().timeIcon(), QString::number(0), QSize(16, 16));

        QVBoxLayout *mainL = new QVBoxLayout;
        QHBoxLayout *topL = new QHBoxLayout;
        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);

        treeButton_ = new QPushButton;
        tableButton_ = new QPushButton;
        textButton_ = new QPushButton;
        treeButton_->setIcon(GuiFactory::instance().treeIcon());
        VERIFY(connect(treeButton_, SIGNAL(clicked()), this, SLOT(setTreeView())));
        tableButton_->setIcon(GuiFactory::instance().tableIcon());
        VERIFY(connect(tableButton_, SIGNAL(clicked()), this, SLOT(setTableView())));
        textButton_->setIcon(GuiFactory::instance().textIcon());
        VERIFY(connect(textButton_, SIGNAL(clicked()), this, SLOT(setTextView())));

        topL->addWidget(treeButton_);
        topL->addWidget(tableButton_);
        topL->addWidget(textButton_);
        topL->addWidget(splitter);
        topL->addWidget(timeLabel_);

        mainL->addLayout(topL);
        mainL->addWidget(treeView_);
        mainL->addWidget(tableView_);
        mainL->addWidget(textView_);
        setLayout(mainL);
        syncWithSettings();
    }

    void OutputWidget::startChangeDbValue(const EventsInfo::ChangeDbValueRequest& req)
    {

    }

    void OutputWidget::finishChangeDbValue(const EventsInfo::ChangeDbValueResponce& res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        commonModel_->changeValue(res.newItem_);
    }

    void OutputWidget::syncWithSettings()
    {
        supportedViews curV = SettingsManager::instance().defaultView();
        if(curV == Tree){
            setTreeView();
        }
        else if(curV == Table){
            setTableView();
        }
        else{
            setTextView();
        }
    }

    void OutputWidget::setTreeView()
    {
        treeView_->setVisible(true);
        tableView_->setVisible(false);
        textView_->setVisible(false);
    }

    void OutputWidget::setTableView()
    {
        treeView_->setVisible(false);
        tableView_->setVisible(true);
        textView_->setVisible(false);
    }

    void OutputWidget::setTextView()
    {
        treeView_->setVisible(false);
        tableView_->setVisible(false);
        textView_->setVisible(true);
    }

    void OutputWidget::rootCreate(const EventsInfo::CommandRootCreatedInfo& res)
    {
        FastoObject* rootObj = res.root_.get();
        fastoredis::FastoCommonItem* root = createItem(NULL, rootObj, "");
        commonModel_->setRoot(root);
    }

    void OutputWidget::rootCompleate(const EventsInfo::CommandRootCompleatedInfo& res)
    {
        timeLabel_->setText(QString("%1 msec").arg(res.elapsedTime()));
    }

    void OutputWidget::addChild(FastoObject *child)
    {
        DCHECK(child->parent());

        QModelIndex parent;
        bool isFound = commonModel_->findItem(child->parent(), parent);
        if(!isFound){
            return;
        }

        fastoredis::FastoCommonItem* par = NULL;
        if(!parent.isValid()){
            par = static_cast<fastoredis::FastoCommonItem*>(commonModel_->root());
        }
        else{
            par = common::utils_qt::item<fastoredis::FastoCommonItem*>(parent);
        }

        fastoredis::FastoCommonItem* comChild = createItem(par, child, par->key());
        commonModel_->insertItem(parent, comChild);
    }

    void OutputWidget::itemUpdate(FastoObject* item, const QString &newValue)
    {
        QModelIndex index;
        bool isFound = commonModel_->findItem(item, index);
        if(!isFound){
            return;
        }

        fastoredis::FastoCommonItem* it = common::utils_qt::item<fastoredis::FastoCommonItem*>(index);
        if(!it){
            return;
        }

        it->setValue(newValue);
        commonModel_->updateItem(index.parent(), index);
    }
}
