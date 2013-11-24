#pragma once

#include <QModelIndex>
#include <QEvent>
#include <QDesktopServices>
#include <QString>
namespace common{
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
		
		template<typename derived_t,typename value_t>
		class Event : public QEvent
		{
		public:
			typedef value_t value_type;
            Event(const value_t &init_value)
                :QEvent((QEvent::Type)derived_t::EventType),value_(init_value)
            {

            }

			void set_value(const value_t &val)
			{
				value_ = val;
			}
			value_t value() const
			{
				return value_;
			}
		private:
			value_t value_;
        };
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QString get_folder_path_by_type(QStandardPaths::StandardLocation loc);
#else
        QString get_folder_path_by_type(QDesktopServices::StandardLocation loc);
#endif
}
}
