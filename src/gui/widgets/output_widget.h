#pragma once

#include <QWidget>

#include "core/events/events_info.h"

class QAction;
class QPushButton;

namespace fastoredis
{
    class FastoEditorView;
    class FastoTreeView;
    class FastoTableView;
    class FastoTreeModel;
    class IconLabel;

    class OutputWidget
            : public QWidget
    {
        Q_OBJECT

    public:
        OutputWidget(QWidget* parent = 0);


    private Q_SLOTS:
        void setTreeView();
        void setTableView();
        void setTextView();

        void rootCreate(const EventsInfo::CommandRootCreatedInfo& res);
        void rootCompleate(const EventsInfo::CommandRootCompleatedInfo& res);
        void addChild(FastoObject *child);

    private:
        void syncWithSettings();
        IconLabel *timeLabel_;
        QPushButton *treeButton_;
        QPushButton *tableButton_;
        QPushButton *textButton_;

        FastoTreeModel *commonModel_;
        FastoTreeView *treeView_;
        FastoTableView *tableView_;
        FastoEditorView *textView_;
    };
}
