#include "common/qt/utils_qt.h"

#include <QApplication>
#include <QDir>

namespace common
{
    namespace utils_qt
    {
        QString applicationDirPath()
        {
            return QApplication::applicationDirPath();
        }
    }
}
