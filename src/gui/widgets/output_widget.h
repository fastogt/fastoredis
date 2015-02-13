#pragma once

#include <QWidget>

#include "core/events/events_info.h"

class QAction;
class QPushButton;

namespace fastoredis
{
    class FastoTextView;
    class FastoTreeView;
    class FastoTableView;
    class FastoCommonModel;
    class IconLabel;

    class OutputWidget
            : public QWidget
    {
        Q_OBJECT
    public:
        OutputWidget(IServerSPtr server, QWidget* parent = 0);

    public Q_SLOTS:
        void rootCreate(const EventsInfo::CommandRootCreatedInfo& res);
        void rootCompleate(const EventsInfo::CommandRootCompleatedInfo& res);
        void startChangeDbValue(const EventsInfo::ChangeDbValueRequest& req);
        void finishChangeDbValue(const EventsInfo::ChangeDbValueResponce& res);
        void addChild(FastoObject* child);
        void itemUpdate(FastoObject* item, const QString& newValue);

    private Q_SLOTS:
        void setTreeView();
        void setTableView();
        void setTextView();

    private:
        void syncWithSettings();
        IconLabel* timeLabel_;
        QPushButton* treeButton_;
        QPushButton* tableButton_;
        QPushButton* textButton_;

        FastoCommonModel* commonModel_;
        FastoTreeView* treeView_;
        FastoTableView* tableView_;
        FastoTextView* textView_;
    };
}
