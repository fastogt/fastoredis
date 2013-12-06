#include "common/qt_helper/converter_patterns.h"
namespace common{
namespace utils_qt
{
    template<>
    QString toQString<std::string>(std::string value)
    {
        return QString::fromUtf8(value.c_str(),value.size());
    }

    template<>
    QString toQString<std::wstring>(std::wstring value)
    {
        return  QString((const QChar*)value.c_str(), value.length());
    }

    template<>
    QString toQString<const char*>(const char *value)
    {
        return QString::fromUtf8(value, strlen(value));
    }

    unicode_string toStdString(const QString &value)
    {
    #ifndef UNICODE
        QByteArray sUtf8 = value.toUtf8();
        return std::string(sUtf8.constData(), sUtf8.length());
    #else
        return std::wstring((wchar_t*)value.unicode(), value.length());
    #endif
    }
}
}
