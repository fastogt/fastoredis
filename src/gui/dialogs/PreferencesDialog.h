#pragma  once

#include <QDialog>

class QComboBox;

#include "global/global.h"

namespace fastoredis
{
    class PreferencesDialog 
		: public QDialog
    {
        Q_OBJECT

    public:
        typedef QDialog BaseClass;
        explicit PreferencesDialog(QWidget *parent);
        enum { height = 640, width = 480};
    public Q_SLOTS:
        virtual void accept();
    private:
        void syncWithSettings();

    private:
        QComboBox *_stylesComboBox;
        QComboBox *_languagesComboBox;
        QComboBox *_defaultViewComboBox;
    };
}
