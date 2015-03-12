#include "gui/dialogs/base_console_dialog.h"

#include <QEvent>
#include <QVBoxLayout>
#include <QToolButton>
#include <QProgressBar>
#include <QSplitter>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QInputDialog>

#include "common/qt/convert_string.h"
#include "common/sprintf.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "gui/fasto_editor.h"

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
            common::SNPrintf(buff, sizeof(buff), PROJECT_NAME" can't read from %s:\n%s.", convertToString(filePath),
                            convertToString(file.errorString()));
            //ErrorValueSPtr er = common::make_error_value(buff, Value::E_ERROR);
            //fastoredis::LOG_ERROR(er);
            QMessageBox::critical(parent, trError,
                QObject::tr(PROJECT_NAME" can't read from %1:\n%2.")
                    .arg(filePath)
                    .arg(file.errorString()));
        }

        return result;
    }

    bool saveToFileText(QString filePath, const QString& text, QWidget* parent, const QString& fileExtension)
    {
        if (filePath.isEmpty()){
            return false;
        }

#ifdef OS_LINUX
        if (QFileInfo(filePath).suffix().isEmpty()) {
            filePath += "." + fileExtension;
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
}

namespace fastoredis
{
    BaseConsoleDialog::BaseConsoleDialog(const QString& filePath, QWidget* parent, const QIcon& icon, bool isExecuteEnabled, FastoEditorShell* shell, const QString& fileExtension)
        : QDialog(parent), shell_(NULL), filePath_(filePath), fileExtension_(fileExtension)
    {
        using namespace translations;
        setWindowIcon(icon);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove help button (?)

        QVBoxLayout *mainLayout = new QVBoxLayout;

        QHBoxLayout *toolBarLayout = new QHBoxLayout;
        toolBarLayout->setContentsMargins(0, 0, 0, 0);

        QToolButton* loadAndInstall = new QToolButton;
        loadAndInstall->setIcon(icon);
        VERIFY(connect(loadAndInstall, &QToolButton::clicked, this, &BaseConsoleDialog::loadAndInstallFile));
        toolBarLayout->addWidget(loadAndInstall);
        loadAndInstall->setEnabled(isExecuteEnabled);

        QToolButton* loadButton = new QToolButton;
        loadButton->setIcon(GuiFactory::instance().loadIcon());
        typedef void (BaseConsoleDialog::*load)();
        VERIFY(connect(loadButton, &QToolButton::clicked, this, static_cast<load>(&BaseConsoleDialog::loadFromFile)));
        toolBarLayout->addWidget(loadButton);

        QToolButton* saveButton = new QToolButton;
        saveButton->setIcon(GuiFactory::instance().saveIcon());
        VERIFY(connect(saveButton, &QToolButton::clicked, this, &BaseConsoleDialog::saveToFile));
        toolBarLayout->addWidget(saveButton);

        QToolButton* saveAsButton = new QToolButton;
        saveAsButton->setIcon(GuiFactory::instance().saveAsIcon());
        VERIFY(connect(saveAsButton, &QToolButton::clicked, this, &BaseConsoleDialog::saveToFileAs));
        toolBarLayout->addWidget(saveAsButton);

        QToolButton* executeButton = new QToolButton;
        executeButton->setIcon(GuiFactory::instance().executeIcon());
        executeButton->setEnabled(isExecuteEnabled);
        executeButton->setPopupMode(QToolButton::MenuButtonPopup);
        QMenu* menu = new QMenu(trMenu, this);
        QAction* execArgs = menu->addAction(trExecuteWithArgs);
        VERIFY(connect(execArgs, &QAction::triggered, this, &BaseConsoleDialog::executeWithArgs));
        executeButton->setMenu(menu);

        VERIFY(connect(executeButton, &QToolButton::clicked, this, static_cast<load>(&BaseConsoleDialog::execute)));
        toolBarLayout->addWidget(executeButton);

        QToolButton *stopButton = new QToolButton;
        stopButton->setIcon(GuiFactory::instance().stopIcon());
        stopButton->setEnabled(isExecuteEnabled);

        VERIFY(connect(stopButton, &QToolButton::clicked, this, &BaseConsoleDialog::stop));
        toolBarLayout->addWidget(stopButton);

        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setContentsMargins(0, 0, 0, 0);
        toolBarLayout->addWidget(splitter);

        workProgressBar_ = new QProgressBar;
        workProgressBar_->setEnabled(isExecuteEnabled);

        toolBarLayout->addWidget(workProgressBar_);

        mainLayout->addLayout(toolBarLayout);

        DCHECK(shell);
        shell_ = shell;
        output_ = new FastoEditor;
        output_->setReadOnly(true);

        mainLayout->addWidget(shell_);
        mainLayout->addWidget(output_);
        setMinimumSize(QSize(min_width, min_height));
        setLayout(mainLayout);
    }

    BaseConsoleDialog::~BaseConsoleDialog()
    {
    }

    void BaseConsoleDialog::loadFromFile()
    {
        loadFromFile(filePath_);
    }

    bool BaseConsoleDialog::loadFromFile(const QString& path)
    {
        bool res = false;
        QString filepath = QFileDialog::getOpenFileName(this, path, QString(),
                                                        QObject::tr("Files (*.%1); All Files (*.*)").arg(fileExtension_));
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

    void BaseConsoleDialog::saveToFileAs()
    {
        QString filepath = QFileDialog::getSaveFileName(this,
            trSaveAs, filePath_, QObject::tr("Files (*.%1); All Files (*.*)").arg(fileExtension_));

        if (saveToFileText(filepath, shell_->text(), this, fileExtension_)) {
            filePath_ = filepath;
        }
    }

    void BaseConsoleDialog::saveToFile()
    {
        if(filePath_.isEmpty()){
            saveToFileAs();
        }
        else {
            saveToFileText(filePath_, shell_->text(), this, fileExtension_);
        }
    }

    void BaseConsoleDialog::executeProgressChanged(int val)
    {
        if(val == 0){
            output_->clear();
        }
        workProgressBar_->setValue(val);
    }

    void BaseConsoleDialog::execute()
    {
        QString selected = shell_->selectedText();
        if(selected.isEmpty()){
            selected = shell_->text();
        }

        execute(selected, QStringList());
    }

    void BaseConsoleDialog::executeWithArgs()
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Args for script"), tr("Args:"), QLineEdit::Normal, QString(), &ok);

        if(ok){
            QString selected = shell_->selectedText();
            if(selected.isEmpty()){
                selected = shell_->text();
            }

            QStringList args = text.split(" ");
            execute(selected, args);
        }
    }

    void BaseConsoleDialog::stop()
    {
        stopImpl();
    }

    void BaseConsoleDialog::execute(const QString& script, const QStringList& args)
    {
        executeImpl(script, args);
    }

    void BaseConsoleDialog::loadAndInstallFile()
    {
        loadAndInstallFileImpl(filePath_);
    }

    void BaseConsoleDialog::changeEvent(QEvent* e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }
        QDialog::changeEvent(e);
    }

    void BaseConsoleDialog::retranslateUi()
    {
    }
}
