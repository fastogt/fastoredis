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
        typedef std::vector<IConnectionSettingsBaseSPtr> cluster_connection_type;
        ClusterDialog(QWidget* parent, IClusterSettingsBase* connection = NULL); //get ownerships connection
        IClusterSettingsBaseSPtr connection() const;

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

        IClusterSettingsBaseSPtr cluster_connection_;
        QLineEdit* connectionName_;
        QComboBox* typeConnection_;
        QCheckBox* logging_;

        QDialogButtonBox *buttonBox_;
    };
}
