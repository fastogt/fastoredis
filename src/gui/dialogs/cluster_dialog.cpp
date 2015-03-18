#include "gui/dialogs/cluster_dialog.h"

#include <QDialogButtonBox>
#include <QEvent>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QTreeWidget>
#include <QToolBar>
#include <QAction>

#include "gui/dialogs/connection_diagnostic_dialog.h"
#include "gui/dialogs/connection_dialog.h"
#include "gui/dialogs/discovery_dialog.h"
#include "gui/dialogs/connection_listwidget_items.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"

#include "translations/global.h"

namespace
{
    const QString defaultNameConnection = "New Cluster Connection";
}

namespace fastoredis
{
    ClusterDialog::ClusterDialog(QWidget* parent, IClusterSettingsBase *connection)
        : QDialog(parent), cluster_connection_(connection)
    {
        using namespace translations;

        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        connectionName_ = new QLineEdit;
        QString conName = defaultNameConnection;
        if(cluster_connection_){
            conName = common::convertFromString<QString>(cluster_connection_->connectionName());
        }
        connectionName_->setText(conName);
        typeConnection_ = new QComboBox;

        for(int i = 0; i < connnectionType.size(); ++i){
            typeConnection_->addItem(common::convertFromString<QString>(connnectionType[i]));
        }

        if(cluster_connection_){
            typeConnection_->setCurrentText(common::convertFromString<QString>(common::convertToString(cluster_connection_->connectionType())));
        }

        VERIFY(connect(typeConnection_, &QComboBox::currentTextChanged, this, &ClusterDialog::typeConnectionChange));

        logging_ = new QCheckBox;
        if(cluster_connection_){
            logging_->setChecked(cluster_connection_->loggingEnabled());
        }
        else{
            logging_->setChecked(false);
        }

        listWidget_ = new QTreeWidget;
        listWidget_->setIndentation(5);

        QStringList colums;
        colums << trName << trAddress;
        listWidget_->setHeaderLabels(colums);
        listWidget_->setContextMenuPolicy(Qt::ActionsContextMenu);
        listWidget_->setIndentation(15);
        listWidget_->setSelectionMode(QAbstractItemView::SingleSelection); // single item can be draged or droped
        listWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);

        if(cluster_connection_){
            IConnectionSettingsBaseSPtr root = cluster_connection_->root();
            if(root){
                addConnection(root);
                IClusterSettingsBase::cluster_connection_type clusters = cluster_connection_->nodes();
                for(IClusterSettingsBase::cluster_connection_type::const_iterator it = clusters.begin(); it != clusters.end(); ++it){
                    IConnectionSettingsBaseSPtr serv = *it;
                    addConnection(serv);
                }
            }
        }

        VERIFY(connect(listWidget_, &QTreeWidget::itemSelectionChanged, this, &ClusterDialog::itemSelectionChanged));

        QHBoxLayout *toolBarLayout = new QHBoxLayout;
        savebar_ = new QToolBar;
        savebar_->setStyleSheet("QToolBar { border: 0px; }");
        toolBarLayout->addWidget(savebar_);

        QAction *addB = new QAction(GuiFactory::instance().loadIcon(), trAddConnection, savebar_);
        typedef void(QAction::*trig)(bool);
        VERIFY(connect(addB, static_cast<trig>(&QAction::triggered), this, &ClusterDialog::add));
        savebar_->addAction(addB);

        QAction *rmB = new QAction(GuiFactory::instance().removeIcon(), trRemoveConnection, savebar_);
        VERIFY(connect(rmB, static_cast<trig>(&QAction::triggered), this, &ClusterDialog::remove));
        savebar_->addAction(rmB);

        QAction *editB = new QAction(GuiFactory::instance().editIcon(), trEditConnection, savebar_);
        VERIFY(connect(editB, static_cast<trig>(&QAction::triggered), this, &ClusterDialog::edit));
        savebar_->addAction(editB);

        QSpacerItem *hSpacer = new QSpacerItem(300, 0, QSizePolicy::Expanding);
        toolBarLayout->addSpacerItem(hSpacer);

        QVBoxLayout *inputLayout = new QVBoxLayout;
        inputLayout->addWidget(connectionName_);
        inputLayout->addWidget(typeConnection_);
        inputLayout->addWidget(logging_);
        inputLayout->addLayout(toolBarLayout);
        inputLayout->addWidget(listWidget_);

        testButton_ = new QPushButton("&Test");
        testButton_->setIcon(GuiFactory::instance().messageBoxInformationIcon());
        VERIFY(connect(testButton_, &QPushButton::clicked, this, &ClusterDialog::testConnection));
        testButton_->setEnabled(false);

        discoveryButton_ = new QPushButton("&Discovery");
        discoveryButton_->setIcon(GuiFactory::instance().discoveryIcon());
        VERIFY(connect(discoveryButton_, &QPushButton::clicked, this, &ClusterDialog::discoveryCluster));
        discoveryButton_->setEnabled(false);

        QHBoxLayout *bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(testButton_, 1, Qt::AlignLeft);
        bottomLayout->addWidget(discoveryButton_, 1, Qt::AlignLeft);
        buttonBox_ = new QDialogButtonBox(this);
        buttonBox_->setOrientation(Qt::Horizontal);
        buttonBox_->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
        VERIFY(connect(buttonBox_, &QDialogButtonBox::accepted, this, &ClusterDialog::accept));
        VERIFY(connect(buttonBox_, &QDialogButtonBox::rejected, this, &ClusterDialog::reject));
        bottomLayout->addWidget(buttonBox_);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(inputLayout);
        mainLayout->addLayout(bottomLayout);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        setLayout(mainLayout);

        //update controls
        typeConnectionChange(typeConnection_->currentText());
        retranslateUi();
    }

    IClusterSettingsBaseSPtr ClusterDialog::connection() const
    {
        return cluster_connection_;
    }

    void ClusterDialog::accept()
    {
        if(validateAndApply()){
            QDialog::accept();
        }
    }

    void ClusterDialog::typeConnectionChange(const QString& value)
    {
        connectionTypes currentType = common::convertFromString<connectionTypes>(common::convertToString(value));
        bool isValidType = currentType == REDIS;
        connectionName_->setEnabled(isValidType);
        buttonBox_->button(QDialogButtonBox::Save)->setEnabled(isValidType);
        savebar_->setEnabled(isValidType);
        listWidget_->setEnabled(isValidType);
    }

    void ClusterDialog::testConnection()
    {
        ConnectionListWidgetItemEx *currentItem = dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem)
            return;

        ConnectionDiagnosticDialog diag(this, currentItem->connection());
        diag.exec();
    }

    void ClusterDialog::discoveryCluster()
    {
        ConnectionListWidgetItemEx* currentItem = dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem){
            return;
        }

        if(!currentItem->isRoot()){
            return;
        }

        DiscoveryDiagnosticDialog diag(this, currentItem->connection());
        int result = diag.exec();
        if(result == QDialog::Accepted){
            std::vector<IConnectionSettingsBaseSPtr> conns = diag.selectedConnections();
            for(int i = 0; i < conns.size(); ++i){
                addConnection(conns[i]);
            }
        }
    }

    void ClusterDialog::add()
    {
        ConnectionDialog dlg(this);
        dlg.setConnectionTypeOnly(REDIS);
        int result = dlg.exec();
        IConnectionSettingsBaseSPtr p = dlg.connection();
        if(result == QDialog::Accepted && p){
            addConnection(p, listWidget_->topLevelItemCount() == 0);
        }
    }

    void ClusterDialog::remove()
    {
        ConnectionListWidgetItemEx *currentItem =
                    dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem)
            return;

        // Ask user
        int answer = QMessageBox::question(this, "Connections", QString("Really delete \"%1\" connection?").arg(currentItem->text(0)),
                                           QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);

        if (answer != QMessageBox::Yes)
            return;

        delete currentItem;
    }

    void ClusterDialog::edit()
    {
        ConnectionListWidgetItemEx *currentItem = dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->currentItem());

        // Do nothing if no item selected
        if (!currentItem)
            return;

        IConnectionSettingsBaseSPtr oldConnection = currentItem->connection();

        ConnectionDialog dlg(this, dynamic_cast<IConnectionSettingsBase*>(oldConnection->clone()));
        dlg.setConnectionTypeOnly(REDIS);
        int result = dlg.exec();
        IConnectionSettingsBaseSPtr newConnection = dlg.connection();
        if(result == QDialog::Accepted && newConnection){
            bool isRoot = currentItem->isRoot();
            delete currentItem;
            addConnection(newConnection, isRoot);
        }
    }

    void ClusterDialog::itemSelectionChanged()
    {
        ConnectionListWidgetItemEx *currentItem = dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->currentItem());

        bool isValidConnection = currentItem != NULL;
        bool isRoot = currentItem != NULL && currentItem->isRoot();

        testButton_->setEnabled(isValidConnection);
        discoveryButton_->setEnabled(isRoot);
    }

    void ClusterDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void ClusterDialog::retranslateUi()
    {
        logging_->setText(tr("Logging enabled"));
    }

    bool ClusterDialog::validateAndApply()
    {
        connectionTypes currentType = common::convertFromString<connectionTypes>(common::convertToString(typeConnection_->currentText()));
        bool isValidType = currentType != DBUNKNOWN;
        if(isValidType){
            std::string conName = common::convertToString(connectionName_->text());
            IClusterSettingsBase* newConnection = IClusterSettingsBase::createFromType(currentType, conName);
            if(newConnection){
                cluster_connection_.reset(newConnection);
                cluster_connection_->setLoggingEnabled(logging_->isChecked());
                for(int i = 0; i < listWidget_->topLevelItemCount(); ++i){
                    ConnectionListWidgetItemEx* item = dynamic_cast<ConnectionListWidgetItemEx *>(listWidget_->topLevelItem(i));
                    if(item){
                        IConnectionSettingsBaseSPtr con = item->connection();
                        if(item->isRoot()){
                            cluster_connection_->setRoot(con);
                        }
                        else{
                            cluster_connection_->addNode(con);
                        }
                    }
                }
            }
            return true;
        }
        else{
            using namespace translations;
            QMessageBox::critical(this, trError, QObject::tr("Invalid database type!"));
            return false;
        }
    }

    void ClusterDialog::addConnection(IConnectionSettingsBaseSPtr con, bool isRoot)
    {
        ConnectionListWidgetItemEx *item = new ConnectionListWidgetItemEx(con, isRoot);
        listWidget_->addTopLevelItem(item);
    }
}
