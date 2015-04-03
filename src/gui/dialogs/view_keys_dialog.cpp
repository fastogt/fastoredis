#include "gui/dialogs/view_keys_dialog.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QScrollBar>
#include <QSplitter>

#include "common/qt/convert_string.h"

#include "core/iserver.h"
#include "core/idatabase.h"

#include "gui/keys_table_model.h"
#include "gui/fasto_table_view.h"
#include "gui/gui_factory.h"

#include "translations/global.h"

namespace
{
    QPushButton *createButtonWithIcon(const QIcon &icon)
    {
        QPushButton *button = new QPushButton;
        button->setIcon(icon);
        button->setFixedSize(24, 24);
        button->setFlat(true);
        return button;
    }
}

namespace fastoredis
{
    ViewKeysDialog::ViewKeysDialog(const QString &title, IDatabaseSPtr db, QWidget* parent)
        : QDialog(parent), db_(db), cursorStack_(), curPos_(0)
    {
        DCHECK(db_);
        if(db_){
            IServerSPtr serv = db_->server();
            VERIFY(connect(serv.get(), &IServer::startedLoadDataBaseContent, this, &ViewKeysDialog::startLoadDatabaseContent));
            VERIFY(connect(serv.get(), &IServer::finishedLoadDatabaseContent, this, &ViewKeysDialog::finishLoadDatabaseContent));
        }
        using namespace translations;

        setWindowTitle(title);

        // main layout
        QVBoxLayout *mainlayout = new QVBoxLayout;

        QHBoxLayout* searchLayout = new QHBoxLayout;
        searchBox_ = new QLineEdit;
        searchBox_->setText("*");
        VERIFY(connect(searchBox_, &QLineEdit::textChanged, this, &ViewKeysDialog::searchLineChanged));
        searchLayout->addWidget(searchBox_);

        countSpinEdit_ = new QSpinBox;
        countSpinEdit_->setRange(min_key_on_page, max_key_on_page);
        countSpinEdit_->setSingleStep(step_keys_on_page);
        countSpinEdit_->setValue(defaults_key);

        keyCountLabel_ = new QLabel;

        searchLayout->addWidget(keyCountLabel_);
        searchLayout->addWidget(countSpinEdit_);

        QPushButton* searchButton = new QPushButton(trSearch);
        VERIFY(connect(searchButton, &QPushButton::clicked, this, &ViewKeysDialog::rightPageClicked));
        searchLayout->addWidget(searchButton);

        keysModel_ = new KeysTableModel(this);
        keysTable_ = new FastoTableView;
        keysTable_->setModel(keysModel_);

        QDialogButtonBox* buttonBox = new QDialogButtonBox;
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        VERIFY(connect(buttonBox, &QDialogButtonBox::accepted, this, &ViewKeysDialog::accept));
        VERIFY(connect(buttonBox, &QDialogButtonBox::rejected, this, &ViewKeysDialog::reject));
        mainlayout->addLayout(searchLayout);
        mainlayout->addWidget(keysTable_);

        leftButtonList_ = createButtonWithIcon(GuiFactory::instance().leftIcon());
        rightButtonList_ = createButtonWithIcon(GuiFactory::instance().rightIcon());
        VERIFY(connect(leftButtonList_, &QPushButton::clicked, this, &ViewKeysDialog::leftPageClicked));
        VERIFY(connect(rightButtonList_, &QPushButton::clicked, this, &ViewKeysDialog::rightPageClicked));
        QHBoxLayout* pagingLayout = new QHBoxLayout;
        QSplitter* splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setContentsMargins(0, 0, 0, 0);
        pagingLayout->addWidget(leftButtonList_);
        pagingLayout->addWidget(splitter);
        pagingLayout->addWidget(rightButtonList_);

        mainlayout->addLayout(pagingLayout);
        mainlayout->addWidget(buttonBox);

        setMinimumSize(QSize(min_width, min_height));
        setLayout(mainlayout);
        retranslateUi();
    }

    void ViewKeysDialog::startLoadDatabaseContent(const EventsInfo::LoadDatabaseContentRequest& req)
    {
        keysModel_->clear();
    }

    void ViewKeysDialog::finishLoadDatabaseContent(const EventsInfo::LoadDatabaseContentResponce& res)
    {
        common::ErrorValueSPtr er = res.errorInfo();
        if(er && er->isError()){
            return;
        }

        if(cursorStack_.size() == curPos_){
            cursorStack_.push_back(res.cursorOut_);
        }

        if(!keysModel_){
            return;
        }

        EventsInfo::LoadDatabaseContentResponce::keys_cont_type keys = res.keys_;

        for(int i = 0; i < keys.size(); ++i){
            NKey key = keys[i];
            keysModel_->insertItem(new KeyTableItem(key));
        }
    }

    void ViewKeysDialog::search(bool forward)
    {
        if(!db_){
            return;
        }

        QString pattern = searchBox_->text();
        if(pattern.isEmpty()){
            return;
        }

        if(cursorStack_.empty()){
            cursorStack_.push_back(0);
        }

        DCHECK(cursorStack_[0] == 0);
        if(forward){
            db_->loadContent(common::convertToString(pattern), countSpinEdit_->value(), cursorStack_[curPos_]);
            ++curPos_;
        }
        else{
            if(curPos_ > 0){
                db_->loadContent(common::convertToString(pattern), countSpinEdit_->value(), cursorStack_[--curPos_]);
            }
        }
    }

    void ViewKeysDialog::searchLineChanged(const QString& text)
    {
        cursorStack_.clear();
        curPos_ = 0;
    }

    void ViewKeysDialog::leftPageClicked()
    {
        search(false);
    }

    void ViewKeysDialog::rightPageClicked()
    {
        search(true);
    }

    void ViewKeysDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void ViewKeysDialog::retranslateUi()
    {
        using namespace translations;
        keyCountLabel_->setText(trKeyCountOnThePage);
    }
}
