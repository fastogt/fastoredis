#pragma once

#include <QModelIndex>
#include <QEvent>
#include <QDesktopServices>

namespace common
{
    namespace utils_qt
    {
        template<typename Type>
        inline Type item(const QModelIndex& index)
        {
            void* raw = index.internalPointer();
            Type stype = static_cast<Type>(raw);
            if(!stype){
                return NULL;
            }

            Type dtype = dynamic_cast<Type>(stype);
            return dtype;
        }

        template<typename Type>
        inline Type get_item(const QModelIndex& index, Type root)
        {
            Type result = root;
            if (index.isValid()){
                result = item<Type>(index);
            }
            return result;
        }

        template<typename value_t, unsigned event_t>
        class Event
                : public QEvent
        {
        public:
            typedef value_t value_type;
            typedef QObject* const senders_type;
            enum { EventType = event_t };

            Event(senders_type sender, const value_t& initValue)
                : QEvent(static_cast<QEvent::Type>(EventType)), value_(initValue), sender_(sender)
            {

            }

            const value_t& value() const
            {
                return value_;
            }

            senders_type sender() const
            {
                return sender_;
            }

        private:
            const value_t value_;
            senders_type sender_;
        };

        template<typename error_t>
        class EventInfo
        {
        public:
            typedef error_t error_type;

            EventInfo(const error_type& er)
                : errorInfo_(er)
            {

            }

            error_type errorInfo() const
            {
                return errorInfo_;
            }

            void setErrorInfo(error_type er)
            {
                errorInfo_ = er;
            }

        protected:
            error_type errorInfo_;
        };

        const QString& applicationDirPath();
    }
}
