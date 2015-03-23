#pragma once

#include <QObject>
#include <QString>

#include "common/patterns/singleton_pattern.h"
#include "common/log_levels.h"
#include "common/value.h"

namespace fastoredis
{
    class Logger
        : public QObject, public common::patterns::LazySingleton<Logger>
    {
        friend class common::patterns::LazySingleton<Logger>;
        Q_OBJECT

    public:
        void print(const char* mess, common::logging::LEVEL_LOG level, bool notify);
        void print(const QString& mess, common::logging::LEVEL_LOG level, bool notify);
        void print(const std::string& mess, common::logging::LEVEL_LOG level, bool notify);

    Q_SIGNALS:
        void printed(const QString& mess, common::logging::LEVEL_LOG level);

    private:
        Logger();
    };

    template<typename T>
    inline void LOG_MSG(T mess, common::logging::LEVEL_LOG level, bool notify)
    {
        return Logger::instance().print(mess, level, notify);
    }

    inline void LOG_ERROR(common::ErrorValueSPtr er, bool notify)
    {
        return Logger::instance().print(er->description(), er->level(), notify);
    }
}
