#include "common/qt/convert_string.h"

namespace common
{
    template<>
    QString convertFromString16<QString>(const string16& value)
    {
        return QString::fromUtf8((const char*)value.c_str(),value.size());
    }


    template<>
    QString convertFromString<QString>(const std::string& value)
    {
        return QString::fromUtf8((const char*)value.c_str(),value.size());
    }

    /*template<>
    QString convertFromString16<std::wstring>(std::wstring value)
    {
        return QString((const QChar*)value.c_str(), value.length());
    }

    template<>
    QString convertFromString16<const char*>(const char *value)
    {
        return QString::fromUtf8(value, strlen(value));
    }*/

    string16 convertToString16(const QString &value)
    {
        QByteArray sUtf8 = value.toUtf8();
        return string16((const char16*)sUtf8.constData(), sUtf8.length());
    }

    std::string convertToString(const QString& from)
    {
        QByteArray sUtf8 = from.toUtf8();
        return std::string(sUtf8.constData(), sUtf8.length());
    }
}
