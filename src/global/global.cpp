#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/utils.h"

namespace
{
    const char *supportedViewsM[] = {"Tree", "Table", "Text"};
}

namespace fastoredis
{
    FastoObject::FastoObject(const FastoObjectPtr &parent, common::Value *val)
        : parent_(parent), value_(val)
    {

    }

    FastoObject::~FastoObject()
    {
        delete value_;
    }

    common::Value::Type FastoObject::type() const
    {
        return value_->GetType();
    }

    std::string FastoObject::toStdString() const
    {
        std::string result;
        value_->GetAsString(&result);
        return result;
    }

    FastoObjectPtr FastoObject::createRoot(const std::string &text)
    {
        FastoObjectPtr result;
        #pragma message("implement plz")
        return result;
    }

    void FastoObject::addChildren(const FastoObjectPtr &child)
    {
        if(child){
            childrens_.push_back(child);
        }
    }

    bool FastoObject::isRoot() const
    {
        return type() == common::Value::TYPE_ARRAY || type() == common::Value::TYPE_ROOT;
    }

    std::string toStdString(const FastoObjectPtr &obj)
    {
        std::string result;
#pragma message("implement plz")
        return result;
    }

    FastoObject::child_container_type FastoObject::childrens() const
    {
        return childrens_;
    }

    std::string toStdString(supportedViews v)
    {
        std::string result;
        int count = sizeof(supportedViewsM)/sizeof(*supportedViewsM);
        if(v < count){
            result = supportedViewsM[v];
        }
        return result;
    }

    supportedViews toSupportedViews(const std::string &text)
    {
        return common::utils::enums::findTypeInArray<supportedViews>(supportedViewsM,text.c_str());
    }

    std::vector<std::string> allSupportedViews()
    {
        return common::utils::enums::convertToVector(supportedViewsM);
    }
}
