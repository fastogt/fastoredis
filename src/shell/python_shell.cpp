#include "shell/python_shell.h"

#include <Python.h>

#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>

#include <QKeyEvent>
#include <QMenu>

#include "gui/shortcuts.h"
#include "gui/gui_factory.h"

namespace
{
    struct PythonInit
    {
        PythonInit()
        {
            Py_SetProgramName(PROJECT_NAME);  /* optional but recommended */
            Py_Initialize();
        }

        ~PythonInit()
        {
            Py_Finalize();
        }
    };
}

namespace fastoredis
{
    const QColor PythonShell::paperColor = QColor(166, 190, 233);

    PythonShell::PythonShell(QWidget* parent)
        : FastoEditor(parent)
    {
        QsciLexerPython* lexer = new QsciLexerPython(this);
        QsciAPIs* api = new QsciAPIs(lexer);
        lexer->setAPIs(api);
        setLexer(lexer);

        setAutoCompletionThreshold(1);
        setAutoCompletionSource(QsciScintilla::AcsAll);
        setAutoCompletionCaseSensitivity(false);
        setIndentationsUseTabs(false);

        setIndentationWidth(4);
        setTabWidth(4);

        VERIFY(connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &))));
    }

    QString PythonShell::version() const
    {
        return version_;
    }

    void PythonShell::showContextMenu(const QPoint& pt)
    {
        QMenu *menu = createStandardContextMenu();
        menu->exec(mapToGlobal(pt));
        delete menu;
    }

    void PythonShell::showAutocompletion()
    {
        autoCompleteFromAll();
    }

    void PythonShell::hideAutocompletion()
    {
        cancelList();
    }

    void PythonShell::keyPressEvent(QKeyEvent* keyEvent)
    {
        if(isAutoCompleteShortcut(keyEvent)){
            showAutocompletion();
            return;
        }
        else if(isHideAutoCompleteShortcut(keyEvent)){
            hideAutocompletion();
            return;
        }

        return FastoEditor::keyPressEvent(keyEvent);
    }
}

