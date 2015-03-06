#include "gui/dialogs/create_dbkey_dialog.h"

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMenu>
#include <QInputDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QEvent>

#include "common/qt/convert_string.h"
#include "gui/gui_factory.h"
#include "translations/global.h"

#include "core/redis/redis_infos.h"
#include "core/memcached/memcached_infos.h"
#include "core/ssdb/ssdb_infos.h"

namespace fastoredis
{
    CreateDbKeyDialog::CreateDbKeyDialog(const QString &title, connectionTypes type, QWidget* parent)
        : QDialog(parent), type_(type), value_()
    {
        using namespace translations;

        setWindowIcon(GuiFactory::instance().icon(type_));
        setWindowTitle(title);

        QGridLayout *kvLayout = new QGridLayout;

        kvLayout->addWidget(new QLabel(tr("Type:")), 0, 0);
        typesCombo_ = new QComboBox;
        if(type_ == REDIS){
            for(int i = 0; i < DBTraits<REDIS>::supportedTypes.size(); ++i){
                common::Value::Type t = DBTraits<REDIS>::supportedTypes[i];
                QString type = common::convertFromString<QString>(common::Value::toString(t));
                typesCombo_->addItem(GuiFactory::instance().icon(t), type, t);
            }
        }
        else if(type_ == MEMCACHED){
            for(int i = 0; i < DBTraits<MEMCACHED>::supportedTypes.size(); ++i){
                common::Value::Type t = DBTraits<MEMCACHED>::supportedTypes[i];
                QString type = common::convertFromString<QString>(common::Value::toString(t));
                typesCombo_->addItem(GuiFactory::instance().icon(t), type, t);
            }
        }
        else if(type_ == SSDB){
            for(int i = 0; i < DBTraits<SSDB>::supportedTypes.size(); ++i){
                common::Value::Type t = DBTraits<SSDB>::supportedTypes[i];
                QString type = common::convertFromString<QString>(common::Value::toString(t));
                typesCombo_->addItem(GuiFactory::instance().icon(t), type, t);
            }
        }
        else{
            NOTREACHED();
        }

        typedef void (QComboBox::*ind)(int);
        VERIFY(connect(typesCombo_, static_cast<ind>(&QComboBox::currentIndexChanged), this, &CreateDbKeyDialog::typeChanged));
        kvLayout->addWidget(typesCombo_, 0, 1);

        //key layout

        kvLayout->addWidget(new QLabel(tr("Key:")), 1, 0);
        keyEdit_ = new QLineEdit;
        kvLayout->addWidget(keyEdit_, 1, 1);

        //value layout

        kvLayout->addWidget(new QLabel(tr("Value:")), 2, 0);
        valueEdit_ = new QLineEdit;
        kvLayout->addWidget(valueEdit_, 2, 1);
        valueEdit_->setVisible(false);

        valueListEdit_ = new QListWidget;
        valueListEdit_->setContextMenuPolicy(Qt::ActionsContextMenu);
        valueListEdit_->setSelectionMode(QAbstractItemView::SingleSelection);

        QAction* addItem = new QAction(trAddItem, valueListEdit_);
        VERIFY(connect(addItem, &QAction::triggered, this, &CreateDbKeyDialog::addItem));
        valueListEdit_->addAction(addItem);

        QAction* removeItem = new QAction(trRemoveItem, valueListEdit_);
        VERIFY(connect(removeItem, &QAction::triggered, this, &CreateDbKeyDialog::removeItem));
        valueListEdit_->addAction(removeItem);

        kvLayout->addWidget(valueListEdit_, 2, 1);
        valueListEdit_->setVisible(false);

        generalBox_ = new QGroupBox;
        generalBox_->setLayout(kvLayout);

        // main layout
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(generalBox_);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        VERIFY(connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateDbKeyDialog::accept));
        VERIFY(connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateDbKeyDialog::reject));
        layout->addWidget(buttonBox);

        typeChanged(0);
        setMinimumSize(QSize(min_width, min_height));
        setLayout(layout);
        retranslateUi();
    }

    bool CreateDbKeyDialog::validateAndApply()
    {
        if(keyEdit_->text().isEmpty()){
            return false;
        }

        FastoObject* obj = getItem();
        if(!obj){
            return false;
        }

        value_.reset(obj);
    }

    void CreateDbKeyDialog::retranslateUi()
    {
        generalBox_->setTitle(tr("Key/Value setup:"));
    }

    void CreateDbKeyDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void CreateDbKeyDialog::typeChanged(int index)
    {
        QVariant var = typesCombo_->itemData(index);
        common::Value::Type t = (common::Value::Type)qvariant_cast<unsigned char>(var);
        if(t == common::Value::TYPE_ARRAY){
            valueListEdit_->setVisible(true);
            valueEdit_->setVisible(false);
        }
        else{
            valueEdit_->setVisible(true);
            valueListEdit_->setVisible(false);
        }
    }

    FastoObjectIPtr CreateDbKeyDialog::value() const
    {
        return value_;
    }

    NKey CreateDbKeyDialog::key() const
    {
        int index = typesCombo_->currentIndex();
        QVariant var = typesCombo_->itemData(index);
        common::Value::Type t = (common::Value::Type)qvariant_cast<unsigned char>(var);
        return NKey(common::convertToString(keyEdit_->text()), t);
    }

    void CreateDbKeyDialog::addItem()
    {
        using namespace translations;

        bool ok;
        QString text = QInputDialog::getText(this, trAddItem, trValue, QLineEdit::Normal, QString(), &ok);
        if (ok && !text.isEmpty()){
            QListWidgetItem* nitem = new QListWidgetItem(text, valueListEdit_);
            nitem->setFlags(nitem->flags() | Qt::ItemIsEditable);
            valueListEdit_->addItem(nitem);
        }
    }

    void CreateDbKeyDialog::removeItem()
    {
        QListWidgetItem* ritem = valueListEdit_->currentItem();
        delete ritem;
    }

    FastoObject* CreateDbKeyDialog::getItem() const
    {
        int index = typesCombo_->currentIndex();
        QVariant var = typesCombo_->itemData(index);
        common::Value::Type t = (common::Value::Type)qvariant_cast<unsigned char>(var);
        if(t == common::Value::TYPE_ARRAY){
            if(valueListEdit_->count() == 0) {
                return NULL;
            }
            common::ArrayValue* ar = common::Value::createArrayValue();
            for(int i = 0; i < valueListEdit_->count(); ++i){
                std::string val = common::convertToString(valueListEdit_->item(i)->text());
                ar->appendString(val);
            }

            return new FastoObjectArray(NULL, ar, " ");
        }
        else if(t == common::Value::TYPE_SET){
            if(valueListEdit_->count() == 0) {
                return NULL;
            }
            common::SetValue* ar = common::Value::createSetValue();
            for(int i = 0; i < valueListEdit_->count(); ++i){
                std::string val = common::convertToString(valueListEdit_->item(i)->text());
                ar->insertString(val);
            }

            return new FastoObjectSet(NULL, ar, " ");
        }
        else if(t == common::Value::TYPE_SET){
            if(valueListEdit_->count() == 0) {
                return NULL;
            }
            common::ZSetValue* ar = common::Value::createZSetValue();
            for(int i = 0; i < valueListEdit_->count(); ++i){
                //std::string val = common::convertToString(valueListEdit_->item(i)->text());
                //ar->insertString(val);
            }

            return new FastoObjectZSet(NULL, ar, " ");
        }
        else{
            QString text = valueEdit_->text();
            if(text.isEmpty()){
                return NULL;
            }

            return new FastoObject(NULL, common::Value::createStringValue(common::convertToString(text)), std::string());
        }
    }

    void CreateDbKeyDialog::accept()
    {
        if(validateAndApply()){
            QDialog::accept();
        }
    }
}
