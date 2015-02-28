#include "gui/dialogs/connection_diagnostic_dialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QThread>
#include <QLabel>

#include "common/time.h"

#include "core/redis/redis_driver.h"
#include "core/ssdb/ssdb_driver.h"
#include "core/memcached/memcached_driver.h"

#include "gui/icon_label.h"
#include "gui/gui_factory.h"
#include "gui/glass_widget.h"

#include "translations/global.h"

namespace
{
    const QString timeTemplate = "Time execute msec: %1";
    const QString connectionStatusTemplate = "Connection state: %1";
}

namespace fastoredis
{
    TestConnection::TestConnection(IConnectionSettingsBaseSPtr conn, QObject* parent)
        : QObject(parent), connection_(conn), startTime_(common::time::current_mstime())
    {

    }

    void TestConnection::routine()
    {
        if(!connection_){
            emit connectionResult(false, common::time::current_mstime() - startTime_, "Invalid connection settings");
            return;
        }

        connectionTypes type = connection_->connectionType();
        common::ErrorValueSPtr er;
        if(type == REDIS){
            er = testConnection(dynamic_cast<RedisConnectionSettings*>(connection_.get()));
        }
        else if(type == MEMCACHED){
            er = testConnection(dynamic_cast<MemcachedConnectionSettings*>(connection_.get()));
        }
        else if(type == SSDB){
            er = testConnection(dynamic_cast<SsdbConnectionSettings*>(connection_.get()));
        }
        else{
            er = common::make_error_value("Invalid setting type", common::ErrorValue::E_ERROR);
        }

        if(er){
            emit connectionResult(false, common::time::current_mstime() - startTime_, common::convertFromString<QString>(er->description()));
        }
        else{
            emit connectionResult(true, common::time::current_mstime() - startTime_, "Success");
        }
    }

    ConnectionDiagnosticDialog::ConnectionDiagnosticDialog(QWidget* parent, IConnectionSettingsBaseSPtr connection)
        : QDialog(parent)
    {
        using namespace translations;

        setWindowTitle(trConnectionDiagnostic);
        setWindowIcon(GuiFactory::instance().serverIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        QVBoxLayout* mainLayout = new QVBoxLayout;

        executeTimeLabel_ = new QLabel;
        executeTimeLabel_->setText(connectionStatusTemplate.arg("execute..."));
        mainLayout->addWidget(executeTimeLabel_);

        statusLabel_ = new IconLabel(GuiFactory::instance().failIcon(), timeTemplate.arg("calculate..."), QSize(32, 32));
        mainLayout->addWidget(statusLabel_);

        QDialogButtonBox* buttonBox = new QDialogButtonBox;
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);
        setFixedSize(QSize(fix_width, fix_height));
        setLayout(mainLayout);

        glassWidget_ = new GlassWidget(GuiFactory::instance().pathToLoadingGif(), trTryToConnect, 0.5, QColor(111, 111, 100), this);
        testConnection(connection);
    }

    void ConnectionDiagnosticDialog::connectionResult(bool suc, qint64 mstimeExecute, const QString& resultText)
    {
        glassWidget_->stop();

        executeTimeLabel_->setText(timeTemplate.arg(mstimeExecute));
        if(suc){
            statusLabel_->setIcon(GuiFactory::instance().successIcon(), QSize(32, 32));
        }
        statusLabel_->setText(connectionStatusTemplate.arg(resultText));
    }

    void ConnectionDiagnosticDialog::showEvent(QShowEvent* e)
    {
        QDialog::showEvent(e);
        glassWidget_->start();
    }

    void ConnectionDiagnosticDialog::testConnection(IConnectionSettingsBaseSPtr connection)
    {
        QThread* th = new QThread;
        TestConnection* cheker = new TestConnection(connection);
        cheker->moveToThread(th);
        VERIFY(connect(th, &QThread::started, cheker, &TestConnection::routine));
        VERIFY(connect(cheker, &TestConnection::connectionResult, this, &ConnectionDiagnosticDialog::connectionResult));
        VERIFY(connect(cheker, &TestConnection::connectionResult, th, &QThread::quit));
        VERIFY(connect(th, &QThread::finished, cheker, &TestConnection::deleteLater));
        VERIFY(connect(th, &QThread::finished, th, &QThread::deleteLater));
        th->start();
    }
}
