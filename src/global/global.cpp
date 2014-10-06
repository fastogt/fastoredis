#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/utils.h"

namespace
{
    const common::char16 *supportedViewsM[] = { UTEXT("Tree"), UTEXT("Table"), UTEXT("Text") };
}

namespace fastoredis
{
    FastoObject::FastoObject(FastoObject *parent, common::Value *val)
        : parent_(parent), value_(val)
    {
        DCHECK(val);
    }

    FastoObject::~FastoObject()
    {
        for(int i = 0; i < childrens_.size(); ++i){
            FastoObject* item = childrens_[i];
            delete item;
        }
        childrens_.clear();
    }

    common::Value::Type FastoObject::type() const
    {
        return value_->getType();
    }

    common::string16 FastoObject::toString() const
    {
        common::string16 result;
        result = value_->toString();//getAsString(&result);
        return result;
    }

    FastoObject* FastoObject::deepCopy(FastoObject* parent) const
    {
        FastoObject* result = new FastoObject(parent, value_->deepCopy());

        for (child_container_type::const_iterator it = childrens_.begin(); it != childrens_.end(); ++it){
            FastoObject* child = (*it)->deepCopy(result);
            result->addChildren(child);
        }

        return result;
    }

    FastoObject *FastoObject::deepCopyChangeParent(FastoObject *parent) const
    {
        for (child_container_type::const_iterator it = childrens_.begin(); it != childrens_.end(); ++it){
            FastoObject* child = (*it)->deepCopy(parent);
            parent->addChildren(child);
        }

        return parent;
    }

    FastoObject *FastoObject::createRoot(const common::string16 &text)
    {
        return new FastoObject(NULL, common::Value::createStringValue(text));
    }

    void FastoObject::addChildren(FastoObject* child)
    {
        if(child){
            DCHECK(child->parent_ == this);
			childrens_.push_back(child);
        }
    }

	bool FastoObject::isRoot() const
	{
        return !parent_ && type() == common::Value::TYPE_STRING;
    }

	FastoObject::child_container_type FastoObject::childrens() const
    {
        return childrens_;
    }

    std::vector<common::string16> allSupportedViews()
    {
        return common::utils::enums::convertToVector(supportedViewsM);
    }
}

namespace common
{
    string16 convertToString16(fastoredis::supportedViews v)
    {
        string16 result;
        int count = sizeof(supportedViewsM)/sizeof(*supportedViewsM);
        if(v < count){
            result = supportedViewsM[v];
        }
        return result;
    }

    template<>
    fastoredis::supportedViews convertFromString16(const string16& from)
    {
        return common::utils::enums::findTypeInArray<fastoredis::supportedViews>(supportedViewsM, from.c_str());
    }

    string16 convertToString16(fastoredis::FastoObject* obj)
    {
        using namespace fastoredis;
        string16 result;
        if(obj){
            string16 str = obj->toString();
            if(!str.empty()){
                result += common::escapedText(str);
            }
            FastoObject::child_container_type childrens = obj->childrens();
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += convertToString16(*it);
            }
        }
        return result;
    }
}
