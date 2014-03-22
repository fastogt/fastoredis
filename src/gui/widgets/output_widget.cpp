#include "gui/widgets/output_widget.h"

#include <QHBoxLayout>
#include <QMenu>
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
//#include "gui/fasto_table_model.h"
#include "common/qt/convert_string.h"
#include "common/time.h"

namespace
{
    fastoredis::FastoTreeItem *createItem(fastoredis::FastoTreeItem *parent, const fastoredis::FastoObjectPtr &item)
    {
        fastoredis::FastoTreeItem *result = NULL;
        fastoredis::FastoObject::child_container_type cont = item->childrens();
        size_t contSize = cont.size();
        if(contSize){
            char size[128] = {0};            
            sprintf(size, "{%zu}", contSize);
            result = new fastoredis::FastoTreeItem( common::utils_qt::toQString(item->toStdString()), size, item->type(), parent);
        }
        else{
            QString varName = QString("%1)").arg(parent->childrenCount()+1);
            result = new fastoredis::FastoTreeItem( varName ,common::utils_qt::toQString(item->toStdString()), item->type(), parent);
        }

        if(parent){
            parent->addChildren(result);
        }
        return result;
    }

    void parseRedisImpl(fastoredis::FastoTreeItem *root, const fastoredis::FastoObjectPtr &item)
    {
        fastoredis::FastoTreeItem *child = createItem(root, item);
        fastoredis::FastoObject::child_container_type cont = item->childrens();
        for(int i = 0; i < cont.size(); ++i){
            fastoredis::FastoObjectPtr obj = cont[i];
            parseRedisImpl(child, obj);
        }
    }

    fastoredis::FastoTreeItem *parseOutput(const fastoredis::EventsInfo::ExecuteInfoResponce::result_type &res)
    {
        fastoredis::FastoTreeItem *result = createItem(NULL, res);
        fastoredis::FastoObject::child_container_type cont = res->childrens();
        for(int i = 0; i < cont.size(); ++i){
            fastoredis::FastoObjectPtr command = cont[i];
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
        _treeView = new FastoTreeView(this);
        _treeModel = new FastoTreeModel(_treeView);
        _treeView->setModel(_treeModel);

        _tableView = new FastoTableView(this);
         //FastoTableModel* mod = new FastoTableModel(_tableView);
         //mod->setSourceModel(_treeModel);
         _tableView->setModel(_treeModel);

        _textView = new FastoEditor(this);
        _timeLabel = new IconLabel(GuiFactory::instance().timeIcon(),common::utils_qt::toQString(common::time::msTimeToString(0)));

        QVBoxLayout *mainL = new QVBoxLayout;
        QHBoxLayout *topL = new QHBoxLayout;
        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);

        _treeButton = new QPushButton;
        _tableButton = new QPushButton;
        _textButton = new QPushButton;
        _treeButton->setIcon(GuiFactory::instance().treeIcon());
        VERIFY(connect(_treeButton, SIGNAL(clicked()), this, SLOT(setTreeView())));
        _tableButton->setIcon(GuiFactory::instance().tableIcon());
        VERIFY(connect(_tableButton, SIGNAL(clicked()), this, SLOT(setTableView())));
        _textButton->setIcon(GuiFactory::instance().textIcon());
        VERIFY(connect(_textButton, SIGNAL(clicked()), this, SLOT(setTextView())));

        topL->addWidget(_treeButton);
        topL->addWidget(_tableButton);
        topL->addWidget(_textButton);
        topL->addWidget(splitter);
        topL->addWidget(_timeLabel);

        mainL->addLayout(topL);
        mainL->addWidget(_treeView);
        mainL->addWidget(_tableView);
        mainL->addWidget(_textView);
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
        _treeView->setVisible(true);
        _tableView->setVisible(false);
        _textView->setVisible(false);
    }

    void OutputWidget::setTableView()
    {
        _treeView->setVisible(false);
        _tableView->setVisible(true);
        _textView->setVisible(false);
    }

    void OutputWidget::setTextView()
    {
        _treeView->setVisible(false);
        _tableView->setVisible(false);
        _textView->setVisible(true);
    }

    void OutputWidget::startExecute(const EventsInfo::ExecuteInfoRequest &req)
    {

    }

    void OutputWidget::finishExecute(const EventsInfo::ExecuteInfoResponce &res)
    {
        _textView->clear();
        if(res._out){
            FastoTreeItem *root = parseOutput(res._out);
            if(root){
                _treeModel->setRoot(root);
            }
        }
        _timeLabel->setText(common::utils_qt::toQString(common::time::msTimeToString(res.elapsedTime())));
        //FastoObjectPtr ptr = res._out;
        //std::string str = toStdString(ptr);
        //_textView->setText(common::utils_qt::toQString(str));
    }
}
