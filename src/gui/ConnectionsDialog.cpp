#include "gui/ConnectionsDialog.h"

#include <QTreeWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "core/SettingsManager.h"
#include "common/qt_helper/converter_patterns.h"
#include "gui/GuiFactory.h"
#include "core/ConnectionSettings.h"

namespace fastoredis
{
    class ConnectionListWidgetItem
            : public QTreeWidgetItem
    {
    public:
        ConnectionListWidgetItem(IConnectionSettingsBase *connection): connection_(connection) { refreshFields(); }
        IConnectionSettingsBase *connection() { return connection_; }

        void refreshFields()
        {
            setText(0, common::utils_qt::toQString(connection_->connectionName()));
            IConnectionSettingsBase::connectionTypes conType = connection_->connectionType();
            if(conType == IConnectionSettingsBase::REDIS){
                RedisConnectionSettings *red = dynamic_cast<RedisConnectionSettings*>(connection_);
                VERIFY(red);

                setText(1, common::utils_qt::toQString(red->fullAdress()));
                setIcon(0, GuiFactory::instance().redisConnectionIcon());
            }
        }

    private:
        IConnectionSettingsBase *connection_;
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
        toolBarLayout->addWidget(addB);

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
        _connectionItems.push_back(new ConnectionListWidgetItem(con));
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
