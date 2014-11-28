#include "global/global.h"

#include "common/utils.h"

namespace
{
    const char *supportedViewsM[] = { "Tree", "Table", "Text" };
}

namespace fastoredis
{
    FastoObject::FastoObject(FastoObject *parent, common::Value *val, const std::string& delemitr)
        : parent_(parent), value_(val), delemitr_(delemitr), childrens_(), observer_(NULL)
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

    std::string FastoObject::toString() const
    {
        std::string result;
        result = value_->toString();//getAsString(&result);
        return result;
    }

    FastoObject* FastoObject::deepCopy(FastoObject* parent) const
    {
        FastoObject* result = new FastoObject(parent, value_->deepCopy(), delemitr_);

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

    FastoObject *FastoObject::createRoot(const std::string &text, IFastoObjectObserver* observer)
    {
        FastoObject* root =  new FastoObject(NULL, common::Value::createStringValue(text), "");
        root->observer_ = observer;
        return root;
    }

    void FastoObject::addChildren(FastoObject* child)
    {
        if(child){
            DCHECK(child->parent_ == this);
            childrens_.push_back(child);
            if(observer_){
                observer_->addedChildren(child);
                child->observer_ = observer_;
            }
        }
    }

    FastoObject* FastoObject::parent() const
    {
        return parent_;
    }

    void FastoObject::clear()
    {
        for (child_container_type::const_iterator it = childrens_.begin(); it != childrens_.end(); ++it){
            FastoObject* child = (*it);
            delete child;
        }
        childrens_.clear();
    }

    std::string FastoObject::delemitr() const
    {
        return delemitr_;
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

    std::string convertToString(fastoredis::FastoObject* obj)
    {
        using namespace fastoredis;
        std::string result;
        if(obj){
            const std::string str = obj->toString();
            if(!str.empty()){
                result += str + obj->delemitr();
            }
            FastoObject::child_container_type childrens = obj->childrens();
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += convertToString(*it);
            }
        }
        return result;
    }
}
