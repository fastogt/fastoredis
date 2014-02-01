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
    FastoObject::FastoObject(const FastoObjectPtr &parent, common::Value *val)
        : parent_(parent), value_(val)
    {

    }

    FastoObject::~FastoObject()
    {
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
        FastoObjectPtr result(new FastoObject(NULL, common::Value::CreateStringValue(text)));
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

    std::string toStdString(const FastoObjectPtr &obj)
    {
        std::string result;
        if(obj){
            std::string str = obj->toStdString();
            if(!str.empty()){
                result += common::escapedText(str);
            }
			FastoObject::child_container_type childrens = obj->childrens();
			for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += toStdString(*it);
            }            
        }
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
