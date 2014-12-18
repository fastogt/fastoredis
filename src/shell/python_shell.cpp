#include "shell/python_shell.h"

#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>

#include "core/python_engine.h"

namespace fastoredis
{
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
        : FastoEditorShell(PythonEngine::version(), parent)
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

