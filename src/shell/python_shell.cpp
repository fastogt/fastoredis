#include "shell/python_shell.h"

#ifdef PYTHON_ENABLED
#include <Python.h>
#endif

#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>

#include "gui/gui_factory.h"

#define PYTHON_SHELL_VERSION "Unknown"

namespace
{
#ifdef PYTHON_ENABLED
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
#endif
}

namespace fastoredis
{
    bool isPythonEnabled()
    {
#ifdef PYTHON_ENABLED
        return true
#else
        return false;
#endif
    }

    const QColor PythonShell::paperColor = QColor(166, 190, 233);

    class RedisPythonLexer
            : public QsciLexerPython
    {
    public:
        RedisPythonLexer(QObject *parent = 0)
            : QsciLexerPython(parent)
        {
            QsciAPIs* api = new QsciAPIs(this);
            api->prepare();
            setAPIs(api);
        }
    };

    PythonShell::PythonShell(QWidget* parent)
        : FastoEditorShell(PYTHON_SHELL_VERSION, parent)
    {
        RedisPythonLexer* lexer = new RedisPythonLexer(this);
        setLexer(lexer);

        setAutoCompletionThreshold(1);
        setAutoCompletionSource(QsciScintilla::AcsAll);
        setAutoCompletionCaseSensitivity(false);
        setIndentationsUseTabs(false);

        setIndentationWidth(4);
        setTabWidth(4);
    }
}

