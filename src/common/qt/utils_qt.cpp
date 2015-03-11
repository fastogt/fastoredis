#include "common/qt/utils_qt.h"

#include <QApplication>
#include <QDir>

namespace common
{
    namespace utils_qt
    {
        const QString& applicationDirPath()
        {
            static const QString path = QApplication::applicationDirPath();
            return path;
        }
    }
}
