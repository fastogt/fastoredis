#pragma once
#include <QString>
#include "common/macros.h"
namespace common
{
namespace utils_qt
{
template<typename T>
QString toQString(const T &value);

unicode_string toStdString(const QString &value);
}
template<typename T>
struct separator_traits
{
	typedef QString type;
	typedef unicode_char char_type;
#ifdef OS_WIN
    static const char_type separator = UTEXT('/');
    static const char_type* separators()
	{
        return UTEXT("//");
	}
#elif defined OS_POSIX
    static const char_type separator = UTEXT('/');
    static const char_type separators()
    {
        return UTEXT("//");
    }
#endif
};
template<>
struct separator_traits<unicode_string>
{
	typedef unicode_string type;
    typedef type::value_type char_type;
#ifdef OS_WIN
	static const char_type separator = UTEXT('\\');
    static const char_type* separators()
	{
		return UTEXT("\\");
	}
#else
	static const char_type separator = UTEXT('/');
    static const char_type* separators()
	{
        return UTEXT("//");
	}
#endif
};
}
