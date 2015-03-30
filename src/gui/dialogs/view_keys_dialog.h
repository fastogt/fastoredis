#pragma once

#include <QDialog>

#include "core/events/events_info.h"

class QLineEdit;

namespace fastoredis
{
    class FastoTableView;
    class KeysTableModel;

    class ViewKeysDialog
            : public QDialog
    {
        Q_OBJECT
    public:
        enum
        {
            min_height = 200,
            min_width = 320,
            max_key_on_page = 50
        };

        explicit ViewKeysDialog(const QString& title, IDatabaseSPtr db, QWidget* parent = 0);

    private Q_SLOTS:
        void startLoadDatabaseContent(const EventsInfo::LoadDatabaseContentRequest& req);
        void finishLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce& res);
        void search();

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();

        QLineEdit* searchBox_;
        FastoTableView* keysTable_;
        KeysTableModel* keysModel_;
        IDatabaseSPtr db_;
    };
}
