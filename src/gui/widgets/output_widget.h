#pragma once

#include <QWidget>

#include "core/events/events_info.h"

class QAction;
class QPushButton;

namespace fastoredis
{
    class FastoEditor;
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

    public Q_SLOTS:
        void startExecute(const EventsInfo::ExecuteInfoRequest &);
        void finishExecute(const EventsInfo::ExecuteInfoResponce &);

    private Q_SLOTS:
        void setTreeView();
        void setTableView();
        void setTextView();

    private:
        void syncWithSettings();
        IconLabel *timeLabel_;
        QPushButton *treeButton_;
        QPushButton *tableButton_;
        QPushButton *textButton_;

        FastoTreeModel *treeModel_;
        FastoTreeView *treeView_;
        FastoTableView *tableView_;
        FastoEditor *textView_;
    };
}
