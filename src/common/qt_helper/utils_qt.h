#pragma once

#include <QModelIndex>
#include <QEvent>
#include <QDesktopServices>
#include <QString>

namespace common
{
    namespace utils_qt
    {
            template<typename Type>
            inline Type item(const QModelIndex &index)
            {
                return static_cast<Type>(index.internalPointer());
            }
            template<typename Type>
            inline Type get_item(const QModelIndex &index,Type root)
            {
                Type result = root;
                if (index.isValid())
                {
                    Type item = static_cast<Type>(index.internalPointer());
                    if (item)
                        result= item;
                }
                return result;
            }

            template<typename value_t, unsigned event_t>
            class Event : public QEvent
            {
            public:
                typedef value_t value_type;
                typedef QEvent base_class;
                enum { EventType = event_t };

                Event(QObject *const sender, const value_t &initValue)
                    : base_class((base_class::Type)EventType), sender_(sender), value_(initValue){}

                QObject *const sender() const
                {
                    return sender_;
                }

                void setValue(const value_t &val)
                {
                    value_ = val;
                }

                value_t value() const
                {
                    return value_;
                }

            private:
                QObject *const sender_;
                value_t value_;
            };

            template<typename error_t>
            class EventInfo
            {
            public:
                typedef error_t error_type;

                EventInfo(const error_type &er)
                    : errorInfo_(er){}

                error_type errorInfo() const
                {
                    return errorInfo_;
                }

            private:
                error_type errorInfo_;
            };

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QString get_folder_path_by_type(QStandardPaths::StandardLocation loc);
    #else
            QString get_folder_path_by_type(QDesktopServices::StandardLocation loc);
    #endif
    }
}
