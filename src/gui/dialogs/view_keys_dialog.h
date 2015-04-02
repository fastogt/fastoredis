#pragma once

#include <QDialog>

#include "core/events/events_info.h"

class QLineEdit;
class QSpinBox;
class QLabel;
class QScrollBar;

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
            min_key_on_page = 1,
            max_key_on_page = 100,
            defaults_key = 10,
            step_keys_on_page = defaults_key
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

        uint32_t currentCursor_;
        QLineEdit* searchBox_;
        QLabel* keyCountLabel_;
        QSpinBox* countSpinEdit_;

        FastoTableView* keysTable_;
        KeysTableModel* keysModel_;
        QScrollBar* pageScrollBox_;
        IDatabaseSPtr db_;
    };
}
