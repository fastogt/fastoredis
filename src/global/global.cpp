#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/utils.h"

namespace
{
    const char *fastoTypeM[] = {"Unknown",
                                                   "String",
                                                   "Array",
                                                   "Integer",
                                                   "Nil",
                                                   "Status",
                                                   "Error",
                                                   "Root"};

    const char *supportedViewsM[] = {"Tree", "Table", "Text"};
}

namespace fastoredis
{
    FastoObject::FastoObject(const FastoObjectPtr &parent, const value_type *memory, const fastoType &type)
        : _parent(parent), alloc_size_(strlen(memory)+1), size_(alloc_size_-1),
          memory_((value_type*)calloc(alloc_size_,sizeof(value_type))), _type(type)
    {
        DCHECK(memory_);
        memcpy(memory_,memory,size_);
    }

    FastoObject::FastoObject(const FastoObjectPtr &parent, const value_type *memory, uint32_t size, const fastoType &type)
        : _parent(parent), alloc_size_(size+1), size_(size),
          memory_((value_type*)calloc(alloc_size_,sizeof(value_type))), _type(type)
    {
        DCHECK(memory_);
        memcpy(memory_,memory,size_);
    }

    fastoType FastoObject::type() const
    {
        return _type;
    }

    FastoObject::uint32_t FastoObject::size() const
    {
        return size_;
    }

    const FastoObject::value_type *const FastoObject::c_str() const
    {
        return memory_;
    }

    const FastoObject::value_type *FastoObject::begin()const
    {
        return memory_;
    }

    const FastoObject::value_type *FastoObject::end()const
    {
        return memory_+size_;
    }

    void FastoObject::append(value_type c)
    {
        alloc(size_ + 1);
        if(memory_)
        {
            memory_[size_++] = c;
        }
        DCHECK(size_<=alloc_size_);
    }

    void FastoObject::alloc(uint32_t strlen_result)
    {
        if(alloc_size_<strlen_result+1)
        {
            alloc_size_=strlen_result*2+1;
            memory_ = (unicode_char*)realloc(memory_, alloc_size_);
            if(memory_)
            {
               memset(memory_+size_,0,alloc_size_-size_);
            }
        }
    }

    FastoObject::~FastoObject()
    {
        free(memory_);
    }

    FastoObjectPtr FastoObject::createRoot(const value_type *memory)
    {
        FastoObjectPtr result = new FastoObject(NULL,memory,ROOT);
        return result;
    }

    void FastoObject::addChildren(const FastoObjectPtr &child)
    {
        if(child){
            _childrens.push_back(child);
        }
    }

    bool FastoObject::isRoot() const
    {
        return type() == ARRAY || type() == ROOT;
    }

    std::string toStdString(const FastoObjectPtr &obj)
    {
        std::string result;
        if(obj){
            FastoObject::child_container_type childrens = obj->childrens();
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += toStdString(*it);
            }
            if(obj->c_str()){
                result += obj->c_str();
                result += '\n';
            }
        }
        return result;
    }

    FastoObject::child_container_type FastoObject::childrens() const
    {
        return _childrens;
    }

    std::string toStdString(fastoType t)
    {
        std::string result;
        int count = sizeof(fastoTypeM)/sizeof(*fastoTypeM);
        if(t < count){
            result = fastoTypeM[t];
        }
        return result;
    }

    fastoType toFastoType(const std::string &text)
    {
        return common::utils::enums::findTypeInArray<fastoType>(fastoTypeM,text.c_str());
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
