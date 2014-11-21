#include "gui/widgets/output_widget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>

#include "gui/fasto_editor.h"
#include "gui/fasto_table_view.h"
#include "gui/fasto_tree_view.h"
#include "gui/fasto_tree_model.h"
#include "gui/fasto_tree_item.h"
#include "gui/gui_factory.h"
#include "gui/icon_label.h"

#include "core/settings_manager.h"

#include "common/qt/convert_string.h"
#include "common/time.h"

namespace
{
    fastoredis::FastoTreeItem *createItem(fastoredis::FastoTreeItem *parent, const fastoredis::FastoObject* item)
    {
        fastoredis::FastoTreeItem *result = NULL;
        fastoredis::FastoObject::child_container_type cont = item->childrens();
        size_t contSize = cont.size();
        const common::string16 itemData = item->toString16();
        if(contSize){
            char size[128] = {0};            
            sprintf(size, "{%zu}", contSize);
            result = new fastoredis::FastoTreeItem( common::convertFromString16<QString>(itemData), size, item->type(), parent);
        }
        else{
            QString varName = QString("%1)").arg(parent->childrenCount()+1);
            result = new fastoredis::FastoTreeItem( varName ,common::convertFromString16<QString>(itemData), item->type(), parent);
        }

        if(parent){
            parent->addChildren(result);
        }
        return result;
    }

    void parseRedisImpl(fastoredis::FastoTreeItem *root, const fastoredis::FastoObject* item)
    {
        fastoredis::FastoTreeItem *child = createItem(root, item);
        fastoredis::FastoObject::child_container_type cont = item->childrens();
        for(int i = 0; i < cont.size(); ++i){
            fastoredis::FastoObject* obj = cont[i];
            parseRedisImpl(child, obj);
        }
    }

    fastoredis::FastoTreeItem *parseOutput(const fastoredis::FastoObject* res)
    {
        DCHECK(res);
        if(!res){
            return NULL;
        }

        fastoredis::FastoTreeItem* result = createItem(NULL, res);
        fastoredis::FastoObject::child_container_type cont = res->childrens();
        for(int i = 0; i < cont.size(); ++i){
            fastoredis::FastoObject* command = cont[i];
            parseRedisImpl(result, command);
        }
        return result;
    }
}

namespace fastoredis
{
    OutputWidget::OutputWidget(QWidget* parent)
        : QWidget(parent)
    {
        commonModel_ = new FastoTreeModel(this);

        treeView_ = new FastoTreeView;
        treeView_->setModel(commonModel_);

        tableView_ = new FastoTableView;
        tableView_->setModel(commonModel_);

        textView_ = new FastoEditor;
        textView_->setModel(commonModel_);

        timeLabel_ = new IconLabel(GuiFactory::instance().timeIcon(), common::convertFromString16<QString>(common::time::mstime2string(0)), QSize(16, 16));

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

    void OutputWidget::startExecute(const EventsInfo::ExecuteInfoRequest& req)
    {

    }

    void OutputWidget::finishExecute(const EventsInfo::ExecuteInfoResponce& res)
    {
        FastoTreeItem *root = parseOutput(res._out.get());

        commonModel_->setRoot(root);
        QModelIndex rootIndex = commonModel_->index(0, 0);
        treeView_->setRootIndex(rootIndex);
        tableView_->setRootIndex(rootIndex);
        textView_->setRootIndex(rootIndex);

        timeLabel_->setText(tr("Execute milliseconds time: %1").arg(res.elapsedTime()));
    }
}
