#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
class QAction;
class QToolButton;
QT_END_NAMESPACE

#include "core/events/EventsInfo.hpp"

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
        typedef QWidget base_class;
        OutputWidget(QWidget* parent = 0);

    public Q_SLOTS:
        void startExecute(const EventsInfo::ExecuteInfoRequest &);
        void finishExecute(const EventsInfo::ExecuteInfoResponce &);

    private: 
        IconLabel *_timeLabel;
        FastoTreeModel *_treeModel;
        FastoTreeView *_treeView;
        FastoTableView *_tableView;
        FastoEditor *_textView;
    };
}
