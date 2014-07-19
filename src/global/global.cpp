#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/utils.h"

namespace
{
    const common::unicode_char *supportedViewsM[] = { UTEXT("Tree"), UTEXT("Table"), UTEXT("Text") };
}

namespace fastoredis
{
    FastoObject::FastoObject(const FastoObjectPtr &parent, common::Value *val)
        : parent_(parent), value_(val)
    {

    }

    FastoObject::~FastoObject()
    {
    }

    common::Value::Type FastoObject::type() const
    {
        return value_->getType();
    }

    common::unicode_string FastoObject::toString() const
    {
        common::unicode_string result;
        result = value_->toString();//getAsString(&result);
        return result;
    }

    FastoObjectPtr FastoObject::createRoot(const std::string &text)
    {
        FastoObjectPtr result(new FastoObject(NULL, common::Value::createStringValue(text)));
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
        return !parent_ && type() == common::Value::TYPE_STRING;
    }

	FastoObject::child_container_type FastoObject::childrens() const
    {
        return childrens_;
    }

    std::vector<common::unicode_string> allSupportedViews()
    {
        return common::utils::enums::convertToVector(supportedViewsM);
    }
}

namespace common
{
    unicode_string convert2string(fastoredis::supportedViews v)
    {
        unicode_string result;
        int count = sizeof(supportedViewsM)/sizeof(*supportedViewsM);
        if(v < count){
            result = supportedViewsM[v];
        }
        return result;
    }

    template<>
    fastoredis::supportedViews convertfromString(const unicode_string& from)
    {
        return common::utils::enums::findTypeInArray<fastoredis::supportedViews>(supportedViewsM, from.c_str());
    }

    unicode_string convert2string(const fastoredis::FastoObjectPtr &obj)
    {
        using namespace fastoredis;
        unicode_string result;
        if(obj){
            unicode_string str = obj->toString();
            if(!str.empty()){
                result += common::escapedText(str);
            }
            FastoObject::child_container_type childrens = obj->childrens();
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += convert2string(*it);
            }
        }
        return result;
    }
}
