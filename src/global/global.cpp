#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/utils.h"

namespace
{
    const char *supportedViewsM[] = { "Tree", "Table", "Text" };
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

    common::string16 FastoObject::toString16() const
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

    FastoObject* FastoObject::createRoot(const std::string &text)
    {
        return createRoot(common::convertToString16(text));
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

    std::vector<std::string> allSupportedViews()
    {
        return common::utils::enums::convertToVector(supportedViewsM);
    }
}

namespace common
{
    std::string convertToString(fastoredis::supportedViews v)
    {
        if(v < SIZEOFMASS(supportedViewsM)){
            return supportedViewsM[v];
        }
        return std::string();
    }

    template<>
    fastoredis::supportedViews convertFromString(const std::string& from)
    {
        const char* fromPtr = from.c_str();
        return common::utils::enums::findTypeInArray<fastoredis::supportedViews>(supportedViewsM, fromPtr);
    }

    string16 convertToString16(fastoredis::FastoObject* obj)
    {
        using namespace fastoredis;
        string16 result;
        if(obj){
            string16 str = obj->toString16();
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
