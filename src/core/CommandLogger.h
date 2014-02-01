#pragma once

#include <QObject>
#include <QString>

#include "common/patterns/singleton_pattern.hpp"

namespace fastoredis
{
    class CommandLogger
        : public QObject, public common::patterns::lazy_singleton<CommandLogger>
    {
        friend class common::patterns::lazy_singleton<CommandLogger>;
        Q_OBJECT
    public:
        void print(const char *mess);
        void print(const std::string &mess);
        void print(const QString &mess);
    Q_SIGNALS:
        void printed(const QString &mess);

    private:
        CommandLogger();
    };

    template<typename T>
    inline void LOG_COMMAND(T mess)
    {
        return CommandLogger::instance().print(mess);
    }
}
