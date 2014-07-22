#pragma  once

/**/

#include <QDialog>

class QComboBox;
class QCheckBox;
class QLabel;
class QLineEdit;

namespace fastoredis
{
    class PreferencesDialog 
		: public QDialog
    {
        Q_OBJECT

    public:
        explicit PreferencesDialog(QWidget *parent);
        enum { height = 640, width = 480};
    public Q_SLOTS:
        virtual void accept();
    private:
        void syncWithSettings();

    protected:
        virtual void changeEvent(QEvent *);

    private:
        void retranslateUi();
        QLabel *langLabel_;
        QLabel *stylesLabel_;
        QComboBox *stylesComboBox_;
        QComboBox *languagesComboBox_;
        QComboBox *defaultViewComboBox_;
        QCheckBox *syncTabs_;
        QLabel *logDirLabel_;
        QLineEdit* logDirPath_;
    };
}
