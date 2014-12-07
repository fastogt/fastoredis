#include "gui/dialogs/python_console_dialog.h"

#include <QEvent>
#include <QVBoxLayout>
#include <QToolButton>
#include <QProgressBar>
#include <QSplitter>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "shell/python_shell.h"

using namespace fastoredis::translations;

namespace
{
    bool loadFromFileText(const QString& filePath, QString& text, QWidget* parent)
    {
        using namespace common;

        bool result = false;
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            text = in.readAll();
            QApplication::restoreOverrideCursor();
            result = true;
        }
        else {
            static const uint16_t size_buff = 256;
            char buff[size_buff] = {0};
            sprintf(buff, PROJECT_NAME" can't read from %1:\n%2.", convertToString(filePath).c_str(),
                            convertToString(file.errorString()).c_str());
            //ErrorValueSPtr er(new ErrorValue(buff, Value::E_ERROR));
            //fastoredis::LOG_ERROR(er);
            QMessageBox::critical(parent, trError,
                QObject::tr(PROJECT_NAME" can't read from %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    bool saveToFileText(QString filePath, const QString& text, QWidget* parent)
    {
        if (filePath.isEmpty()){
            return false;
        }

#ifdef OS_LINUX
        if (QFileInfo(filePath).suffix().isEmpty()) {
            filePath += ".txt";
        }
#endif
        bool result = false;
        QFile file(filePath);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            out << text;
            QApplication::restoreOverrideCursor();
            result = true;
        }
        else {
            QMessageBox::critical(parent, trError,
                QObject::tr(PROJECT_NAME" can't save to %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    const QSize iconSize = QSize(16, 16);
}

namespace fastoredis
{
    PythonConsoleDialog::PythonConsoleDialog(const QString& filePath, QWidget* parent)
        : QDialog(parent), filePath_(filePath)
    {
        using namespace translations;

        setWindowIcon(GuiFactory::instance().pythonIcon());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        QVBoxLayout *mainLayout = new QVBoxLayout;

        QHBoxLayout *toolBarLayout = new QHBoxLayout;
        toolBarLayout->setContentsMargins(0, 0, 0, 0);

        QToolButton* loadButton = new QToolButton;
        loadButton->setIcon(GuiFactory::instance().loadIcon());
        VERIFY(connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile())));
        toolBarLayout->addWidget(loadButton);

        QToolButton* saveButton = new QToolButton;
        saveButton->setIcon(GuiFactory::instance().saveIcon());
        VERIFY(connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile())));
        toolBarLayout->addWidget(saveButton);

        QToolButton* saveAsButton = new QToolButton;
        saveAsButton->setIcon(GuiFactory::instance().saveAsIcon());
        VERIFY(connect(saveAsButton, SIGNAL(clicked()), this, SLOT(saveToFileAs())));
        toolBarLayout->addWidget(saveAsButton);

        QToolButton* executeButton = new QToolButton;
        executeButton->setIcon(GuiFactory::instance().executeIcon());
        executeButton->setEnabled(isPythonEnabled());

        VERIFY(connect(executeButton, SIGNAL(clicked()), this, SLOT(execute())));
        toolBarLayout->addWidget(executeButton);

        QToolButton *stopButton = new QToolButton;
        stopButton->setIcon(GuiFactory::instance().stopIcon());
        stopButton->setEnabled(isPythonEnabled());

        VERIFY(connect(stopButton, SIGNAL(clicked()), this, SLOT(stop())));
        toolBarLayout->addWidget(stopButton);

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        toolBarLayout->addWidget(splitter);

        workProgressBar_ = new QProgressBar;
        workProgressBar_->setEnabled(isPythonEnabled());

        toolBarLayout->addWidget(workProgressBar_);

        mainLayout->addLayout(toolBarLayout);

        shell_ = new PythonShell;
        mainLayout->addWidget(shell_);
        setMinimumSize(QSize(width, height));
        setLayout(mainLayout);

        retranslateUi();
    }

    void PythonConsoleDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void PythonConsoleDialog::retranslateUi()
    {
        using namespace translations;
        setWindowTitle(trPythonConsole);
    }

    void PythonConsoleDialog::loadFromFile()
    {
        QString out;
        bool res = loadFromFile(filePath_);
        if(res){
            shell_->setText(out);
        }
    }

    bool PythonConsoleDialog::loadFromFile(const QString& path)
    {
        bool res = false;
        QString filepath = QFileDialog::getOpenFileName(this, path, QString(), trfilterForPython);
        if (!filepath.isEmpty()) {
            QString out;
            if (loadFromFileText(filepath, out, this)) {
                shell_->setText(out);
                filePath_ = filepath;
                res = true;
            }
        }
        return res;
    }

    void PythonConsoleDialog::saveToFileAs()
    {
        QString filepath = QFileDialog::getSaveFileName(this,
            trSaveAs, filePath_, trfilterForPython);

        if (saveToFileText(filepath, shell_->text(), this)) {
            filePath_ = filepath;
        }
    }

    void PythonConsoleDialog::saveToFile()
    {
        if(filePath_.isEmpty()){
            saveToFileAs();
        }
        else {
            saveToFileText(filePath_, shell_->text(), this);
        }
    }

    void PythonConsoleDialog::execute()
    {
    }

    void PythonConsoleDialog::stop()
    {
    }
}
