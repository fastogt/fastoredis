#include "gui/FastoTreeModel.h"

namespace fastoredis
{
    FastoTreeModel::FastoTreeModel(const FastoObjectPtr &root, QObject *parent)
        :base_class(parent), _root(root)
    {

    }
}
