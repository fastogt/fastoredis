#pragma once

#include <QIcon>
#include <QFont>

#include "core/connection_types.h"

#include "common/value.h"
#include "common/patterns/singleton_pattern.h"

namespace fastoredis
{
    class GuiFactory
            : public common::patterns::lazy_singleton<GuiFactory>
    {
    public:
        typedef common::patterns::lazy_singleton<GuiFactory> base_class;
        friend class common::patterns::lazy_singleton<GuiFactory>;

        const QIcon& openIcon() const;
        const QIcon& mainWindowIcon() const;
        const QIcon& connectIcon() const;
        const QIcon& disConnectIcon() const;
        const QIcon& redisConnectionIcon() const;
        const QIcon& serverIcon() const;
        const QIcon& addIcon() const;
        const QIcon& removeIcon() const;
        const QIcon& editIcon() const;
        const QIcon& messageBoxInformationIcon() const;
        const QIcon& messageBoxQuestionIcon() const;
        const QIcon& executeIcon() const;
        const QIcon& stopIcon() const;
        const QIcon& timeIcon() const;
        const QIcon& databaseIcon() const;
        QFont font() const;
        const QIcon& icon(connectionTypes type) const;
        const QIcon& modeIcon(ConnectionMode mode) const;
        const QIcon& icon(common::Value::Type type) const;
        const QIcon& loadIcon() const;
        const QIcon& saveIcon() const;
        const QIcon& saveAsIcon() const;
        const QIcon& textIcon() const;
        const QIcon& tableIcon() const;
        const QIcon& treeIcon() const;
        const QString& loadingPathFilePath() const;
        const QIcon& loggingIcon() const;
        const QIcon& commandIcon() const;
        const QIcon& close16Icon() const;
        const QIcon& pythonIcon() const;

        const QIcon& commandIcon(connectionTypes type) const;
        const QIcon& typeIcon(connectionTypes type) const;
    };
}
