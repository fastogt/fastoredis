#include "gui/OutputWidget.h"

#include <QHBoxLayout>
#include <QMenu>

#include "gui/FastoEditor.h"
#include "gui/FastoTableView.h"
#include "gui/FastoTreeView.h"
#include "gui/FastoTreeModel.h"
#include "gui/FastoTreeItem.h"
//#include "gui/FastoTableModel.h"
#include "common/qt_helper/converter_patterns.h"

namespace
{
    fastoredis::FastoTreeItem *createItem(fastoredis::FastoTreeItem *parent, const fastoredis::FastoObjectPtr &item)
    {
        fastoredis::FastoTreeItem *result = NULL;
        if(item->isRoot()){
            char size[128] = {0};
            fastoredis::FastoObject::child_container_type cont = item->childrens();
            sprintf(size, "{%u}", cont.size());
            result = new fastoredis::FastoTreeItem( common::utils_qt::toQString(item->c_str()), size, item->type(), parent);
        }
        else{
            QString varName;
            QString parenKey = parent->key();
            int index = parenKey.startsWith("get", Qt::CaseInsensitive);
            if(index != -1){
                varName = parenKey.mid(index+2);
            }
            result = new fastoredis::FastoTreeItem( varName ,common::utils_qt::toQString(item->c_str()), item->type(), parent);
        }

        if(parent){
            parent->addChildren(result);
        }
        return result;
    }

    void parseRedisImpl(fastoredis::FastoTreeItem *root, const fastoredis::FastoObjectPtr &item)
    {
        fastoredis::FastoTreeItem *child = createItem(root,item);

        fastoredis::FastoObject::child_container_type cont = item->childrens();
        for(fastoredis::FastoObject::child_container_type::const_iterator it = cont.begin(); it != cont.end(); ++it){
            fastoredis::FastoObjectPtr obj = *it;
            parseRedisImpl(child, obj);
        }
    }

    fastoredis::FastoTreeItem *parseOutput(const fastoredis::EventsInfo::ExecuteInfoResponce::result_type &res)
    {
        fastoredis::FastoTreeItem *result = createItem(NULL,res);
        fastoredis::FastoObject::child_container_type cont = res->childrens();
        for(fastoredis::FastoObject::child_container_type::const_iterator it = cont.begin(); it != cont.end(); ++it){
            fastoredis::FastoObjectPtr command = *it;
            parseRedisImpl(result, command);
        }
        return result;
    }
}

namespace fastoredis
{
    OutputWidget::OutputWidget(QWidget* parent)
        : base_class(parent)
    {
        _treeView = new FastoTreeView(this);
        _treeModel = new FastoTreeModel(_treeView);
        _treeView->setModel(_treeModel);

        _tableView = new FastoTableView(this);
         //FastoTableModel* mod = new FastoTableModel(_tableView);
         //mod->setSourceModel(_treeModel);
         _tableView->setModel(_treeModel);

        _textView = new FastoEditor(this);
        QVBoxLayout *mainL = new QVBoxLayout;
        mainL->addWidget(_treeView);
        mainL->addWidget(_tableView);

        mainL->addWidget(_textView);
        setLayout(mainL);
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
        //FastoObjectPtr ptr = res._out;
        //std::string str = toStdString(ptr);
        //_textView->setText(common::utils_qt::toQString(str));
    }
}
