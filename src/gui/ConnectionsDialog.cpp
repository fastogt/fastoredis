#include "gui/ConnectionsDialog.h"

#include <QTreeWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMessageBox>

#include "core/SettingsManager.h"
#include "common/qt_helper/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "core/ConnectionSettings.h"
#include "gui/ConnectionDialog.h"

namespace fastoredis
{
    class ConnectionListWidgetItem
            : public QTreeWidgetItem
    {
    public:
        ConnectionListWidgetItem(const IConnectionSettingsBasePtr &connection): connection_(connection) { refreshFields(); }
        IConnectionSettingsBasePtr connection() { return connection_; }

        void refreshFields()
        {
            setText(0, common::utils_qt::toQString(connection_->connectionName()));
            IConnectionSettingsBase::connectionTypes conType = connection_->connectionType();
            if(conType == IConnectionSettingsBase::REDIS){
                RedisConnectionSettings *red = dynamic_cast<RedisConnectionSettings*>(connection_.get());
                VERIFY(red);

                setText(1, common::utils_qt::toQString(red->fullAdress()));
                setIcon(0, GuiFactory::instance().redisConnectionIcon());
            }
        }

    private:
        IConnectionSettingsBasePtr connection_;
    };

    /**
     * @brief Creates dialog
     */
    ConnectionsDialog::ConnectionsDialog(QWidget *parent)
        : base_class(parent)
    {
        setWindowIcon(GuiFactory::instance().connectIcon());
        setWindowTitle("Redis Connections");

        // Remove help button (?)
        setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);;

        _listWidget = new QTreeWidget;
        _listWidget->setIndentation(5);

        QStringList colums;
        colums << "Name" << "Address" << "Auth. Database / User";
        _listWidget->setHeaderLabels(colums);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        _listWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        _listWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
        _listWidget->header()->setSectionResizeMode(2, QHeaderView::Stretch);
#endif
        //_listWidget->setViewMode(QListView::ListMode);
        _listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
        _listWidget->setIndentation(15);
        _listWidget->setSelectionMode(QAbstractItemView::SingleSelection); // single item can be draged or droped
        _listWidget->setDragEnabled(true);
        _listWidget->setDragDropMode(QAbstractItemView::InternalMove);
        _listWidget->setMinimumHeight(300);
        _listWidget->setMinimumWidth(630);
        VERIFY(connect(_listWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(accept())));;

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        buttonBox->button(QDialogButtonBox::Save)->setIcon(GuiFactory::instance().serverIcon());
        buttonBox->button(QDialogButtonBox::Save)->setText("C&onnect");
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

        QSpacerItem *hSpacer = new QSpacerItem(300,0,QSizePolicy::Expanding);
        toolBarLayout->addSpacerItem(hSpacer);

        QVBoxLayout *firstColumnLayout = new QVBoxLayout;
        firstColumnLayout->addLayout(toolBarLayout);
        firstColumnLayout->addWidget(_listWidget);
        firstColumnLayout->addLayout(bottomLayout);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addLayout(firstColumnLayout, 1);

        // Populate list with connections
        SettingsManager::ConnectionSettingsContainerType connections = SettingsManager::instance().connections();
        for (SettingsManager::ConnectionSettingsContainerType::const_iterator it = connections.begin(); it != connections.end(); ++it) {
            IConnectionSettingsBase *connectionModel = (*it).get();
            add(connectionModel);
        }

        // Highlight first item
        if (_listWidget->topLevelItemCount() > 0)
            _listWidget->setCurrentItem(_listWidget->topLevelItem(0));
    }

    void ConnectionsDialog::add(IConnectionSettingsBase *con)
    {
        ConnectionListWidgetItem *item = new ConnectionListWidgetItem(con);
        _listWidget->addTopLevelItem(item);
        _connectionItems.push_back(item);
    }

    void ConnectionsDialog::add()
    {
        IConnectionSettingsBasePtr p = new RedisConnectionSettings("New Connection");
        ConnectionDialog dlg(p,this);
        int result = dlg.exec();
        if(result == QDialog::Accepted){
            SettingsManager::instance().addConnection(p);
        }
    }

    void ConnectionsDialog::remove()
    {
        ConnectionListWidgetItem *currentItem =
                    dynamic_cast<ConnectionListWidgetItem *>(_listWidget->currentItem());

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

    IConnectionSettingsBase *ConnectionsDialog::selectedConnection() const
    {
        return NULL;
    }

    /**
     * @brief This function is called when user clicks on "Connect" button.
     */
    void ConnectionsDialog::accept()
    {
        QDialog::accept();
    }
}
