#include "gui/dialogs/connections_dialog.h"

#include <QTreeWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMessageBox>
#include <QEvent>

#include "common/qt/convert_string.h"

#include "core/settings_manager.h"
#include "core/connection_settings.h"

#include "gui/gui_factory.h"
#include "gui/dialogs/connection_dialog.h"

#include "translations/global.h"

namespace fastoredis
{
    class ConnectionListWidgetItem
            : public QTreeWidgetItem
    {
    public:
        ConnectionListWidgetItem(const IConnectionSettingsBasePtr& connection): connection_(connection) { refreshFields(); }
        IConnectionSettingsBasePtr connection() { return connection_; }

        void refreshFields()
        {
            setText(0, common::convertFromString<QString>(connection_->connectionName()));
            connectionTypes conType = connection_->connectionType();
            if(conType == REDIS){
                RedisConnectionSettings *red = dynamic_cast<RedisConnectionSettings*>(connection_.get());
                VERIFY(red);

                setText(1, common::convertFromString<QString>(red->fullAddress()));
                setIcon(0, GuiFactory::instance().redisConnectionIcon());
            }
        }

    private:
        IConnectionSettingsBasePtr connection_;
    };

    /**
     * @brief Creates dialog
     */
    ConnectionsDialog::ConnectionsDialog(QWidget* parent)
        : QDialog(parent)
    {
        using namespace translations;

        setWindowIcon(GuiFactory::instance().connectIcon());

        // Remove help button (?)
        setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);;

        listWidget_ = new QTreeWidget;
        listWidget_->setIndentation(5);

        QStringList colums;
        colums << trName << trAddress << trAuthDbUser;
        listWidget_->setHeaderLabels(colums);

        listWidget_->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        listWidget_->header()->setSectionResizeMode(1, QHeaderView::Stretch);
        listWidget_->header()->setSectionResizeMode(2, QHeaderView::Stretch);

        //listWidget_->setViewMode(QListView::ListMode);
        listWidget_->setContextMenuPolicy(Qt::ActionsContextMenu);
        listWidget_->setIndentation(15);
        listWidget_->setSelectionMode(QAbstractItemView::SingleSelection); // single item can be draged or droped
        listWidget_->setDragEnabled(true);
        listWidget_->setDragDropMode(QAbstractItemView::InternalMove);
        listWidget_->setMinimumHeight(300);
        listWidget_->setMinimumWidth(630);
        VERIFY(connect(listWidget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(accept())));
        VERIFY(connect(listWidget_, SIGNAL(itemSelectionChanged()), this, SLOT(connectionSelectChange())));

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        buttonBox->button(QDialogButtonBox::Ok)->setIcon(GuiFactory::instance().serverIcon());
        acButton_ = buttonBox->button(QDialogButtonBox::Ok);

        acButton_->setEnabled(false);

        VERIFY(connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept())));
        VERIFY(connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())));

        QHBoxLayout *bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(buttonBox);

        QHBoxLayout *toolBarLayout = new QHBoxLayout;

        QToolButton *addB = new QToolButton;
        addB->setIcon(GuiFactory::instance().addIcon());
        VERIFY(connect(addB, SIGNAL(clicked()), this, SLOT(add())));
        toolBarLayout->addWidget(addB);

        QToolButton *rmB = new QToolButton;
        rmB->setIcon(GuiFactory::instance().removeIcon());
        VERIFY(connect(rmB, SIGNAL(clicked()), this, SLOT(remove())));
        toolBarLayout->addWidget(rmB);

        QToolButton *editB = new QToolButton;
        editB->setIcon(GuiFactory::instance().editIcon());
        VERIFY(connect(editB, SIGNAL(clicked()), this, SLOT(edit())));
        toolBarLayout->addWidget(editB);

        QSpacerItem *hSpacer = new QSpacerItem(300, 0, QSizePolicy::Expanding);
        toolBarLayout->addSpacerItem(hSpacer);

        QVBoxLayout *firstColumnLayout = new QVBoxLayout;
        firstColumnLayout->addLayout(toolBarLayout);
        firstColumnLayout->addWidget(listWidget_);
        firstColumnLayout->addLayout(bottomLayout);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addLayout(firstColumnLayout, 1);

        // Populate list with connections
        SettingsManager::ConnectionSettingsContainerType connections = SettingsManager::instance().connections();
        for (SettingsManager::ConnectionSettingsContainerType::const_iterator it = connections.begin(); it != connections.end(); ++it) {
            IConnectionSettingsBasePtr connectionModel = (*it);
            add(connectionModel);
        }

        // Highlight first item
        if (listWidget_->topLevelItemCount() > 0)
            listWidget_->setCurrentItem(listWidget_->topLevelItem(0));
        retranslateUi();
    }

    void ConnectionsDialog::connectionSelectChange()
    {
        acButton_->setEnabled(selectedConnection());
    }

    void ConnectionsDialog::add(const IConnectionSettingsBasePtr& con)
    {
        ConnectionListWidgetItem *item = new ConnectionListWidgetItem(con);
        listWidget_->addTopLevelItem(item);
    }

    void ConnectionsDialog::add()
    {
        redisConfig conf;
        IConnectionSettingsBasePtr p(new RedisConnectionSettings("New Connection", conf));
        ConnectionDialog dlg(p,this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            SettingsManager::instance().addConnection(p);
            add(p);
        }
    }

    void ConnectionsDialog::remove()
    {
        ConnectionListWidgetItem *currentItem =
                    dynamic_cast<ConnectionListWidgetItem *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem)
            return;

        // Ask user
        int answer = QMessageBox::question(this,
            "Connections",
            QString("Really delete \"%1\" connection?").arg(currentItem->text(0)),
            QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);

        if (answer != QMessageBox::Yes)
            return;

        IConnectionSettingsBasePtr connection = currentItem->connection();
        delete currentItem;
        SettingsManager::instance().removeConnection(connection);
    }

    void ConnectionsDialog::edit()
    {
        ConnectionListWidgetItem *currentItem =
                    dynamic_cast<ConnectionListWidgetItem *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem)
            return;

        IConnectionSettingsBasePtr oldConnection = currentItem->connection();
        IConnectionSettingsBasePtr newConnection(oldConnection->clone());

        ConnectionDialog dlg(newConnection,this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            delete currentItem;
            SettingsManager::instance().removeConnection(oldConnection);
            SettingsManager::instance().addConnection(newConnection);
            add(newConnection);
        }
    }

    IConnectionSettingsBasePtr ConnectionsDialog::selectedConnection() const
    {
        IConnectionSettingsBasePtr res;

        ConnectionListWidgetItem *currentItem =
                    dynamic_cast<ConnectionListWidgetItem *>(listWidget_->currentItem());
        if (currentItem){
            res = currentItem->connection();
        }

        return res;
    }

    /**
     * @brief This function is called when user clicks on "Connect" button.
     */
    void ConnectionsDialog::accept()
    {
        QDialog::accept();
    }

    void ConnectionsDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void ConnectionsDialog::retranslateUi()
    {
        using namespace translations;

        setWindowTitle(tr("Redis Connections"));
        acButton_->setText(trOpen);
    }
}
