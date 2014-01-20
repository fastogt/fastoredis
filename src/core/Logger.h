#pragma once

#include <QObject>
#include <QString>

#include "common/patterns/singleton_pattern.hpp"
#include "global/global.h"

namespace fastoredis
{
    class Logger
        : public QObject, public common::patterns::lazy_singleton<Logger>
    {
        friend class common::patterns::lazy_singleton<Logger>;
        Q_OBJECT
    public:
        void print(const char *mess, common::logging::LEVEL_LOG level, bool notify);
        void print(const std::string &mess, common::logging::LEVEL_LOG level, bool notify);
        void print(const QString &mess, common::logging::LEVEL_LOG level, bool notify);
    Q_SIGNALS:
        void printed(const QString &mess, common::logging::LEVEL_LOG level);

    private:
        Logger();
    };

    template<typename T>
    inline void LOG_MSG(T mess, common::logging::LEVEL_LOG level, bool notify = true)
    {
        return Logger::instance().print(mess, level, notify);
    }

    inline void LOG_ERROR(const error::ErrorInfo &er, bool notify = true)
    {
        return Logger::instance().print(er.description(), er.level(), notify);
    }
}
