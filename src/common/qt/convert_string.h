#pragma once

#include <QString>

#include "common/convert2string.h"

namespace common
{
        string16 convertToString16(const QString& from);
        std::string convertToString(const QString& from);
        QString escapedText(const QString& str);
        QString doubleEscapedText(QString str);
}
