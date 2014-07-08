#include "common/qt/convert_string.h"

namespace common
{
    template<>
    QString convertfromString<QString>(const unicode_string& value)
    {
        return QString::fromUtf8(value.c_str(),value.size());
    }

    /*template<>
    QString convertfromString<std::wstring>(std::wstring value)
    {
        return QString((const QChar*)value.c_str(), value.length());
    }

    template<>
    QString convertfromString<const char*>(const char *value)
    {
        return QString::fromUtf8(value, strlen(value));
    }*/

    unicode_string convert2string(const QString &value)
    {
#ifndef UNICODE
        QByteArray sUtf8 = value.toUtf8();
        return std::string(sUtf8.constData(), sUtf8.length());
#else
        return std::wstring((wchar_t*)value.unicode(), value.length());
#endif
    }
}
