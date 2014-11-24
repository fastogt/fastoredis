#pragma once

/**/

#include <QString>
#include <QObject>

namespace fastoredis
{
    namespace translations
    {
        const QString trfilterForScripts = QObject::tr("Text Files (*.txt); All Files (*.*)");
        const QString trLoad = QObject::tr("Load");
        const QString trSave = QObject::tr("Save");
        const QString trSaveAs = QObject::tr("Save as");
        const QString trConnect = QObject::tr("Connect");
        const QString trDisconnect = QObject::tr("Disconnect");
        const QString trExecute = QObject::tr("Execute");
        const QString trStop = QObject::tr("Stop");
        const QString trError = QObject::tr("Error");
    }
}
