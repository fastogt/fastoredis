#include "common/qt/utils_qt.h"

#include <QApplication>
#include <QDir>

namespace common
{
    namespace utils_qt
    {
        QString get_folder_path_by_type(QStandardPaths::StandardLocation loc)
        {
            QStringList path = QStandardPaths::standardLocations(loc);
            Q_ASSERT(!path.empty());
            QDir dir;
            dir.mkpath(path[0]);
            return path[0] + QDir::separator();
        }

        QString applicationDirPath()
        {
            return QApplication::applicationDirPath();
        }
    }
}
