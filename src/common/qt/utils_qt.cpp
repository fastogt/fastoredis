#include "common/qt/utils_qt.h"

#include <QDir>

namespace common
{
    namespace utils_qt
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QString get_folder_path_by_type(QStandardPaths::StandardLocation loc)
            {
                QStringList path = QStandardPaths::standardLocations(loc);
                Q_ASSERT(!path.empty());
                QDir dir;
                dir.mkpath(path[0]);
                return path[0] + QDir::separator();
            }
#else
            QString get_folder_path_by_type(QDesktopServices::StandardLocation loc)
            {
                QString path = QDesktopServices::storageLocation(loc);
                Q_ASSERT(!path.isEmpty());
                QDir dir;
                dir.mkpath(path);
                return path + QDir::separator();
            }
#endif
    }
}
