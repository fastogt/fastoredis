#pragma once

#include <QDialog>

class QComboBox;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;
class QCheckBox;
class QLabel;

#include "core/connection_settings.h"

namespace fastoredis
{
    class ClusterDialog
            : public QDialog
    {
        Q_OBJECT

    public:
        ClusterDialog(QWidget* parent, IConnectionSettingsBase* connection = NULL); //get ownerships connection
        IConnectionSettingsBaseSPtr connection() const;

    public Q_SLOTS:
        virtual void accept();

    private Q_SLOTS:
        void typeConnectionChange(const QString& value);
        void testConnection();

    protected:
        virtual void changeEvent(QEvent* );

    private:
        void retranslateUi();
        bool validateAndApply();

        IConnectionSettingsBaseSPtr connection_;
        QLineEdit* connectionName_;
        QComboBox* typeConnection_;
        QCheckBox* logging_;

        QDialogButtonBox *buttonBox_;
    };
}
