#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/macros.h"

namespace fastoredis
{
    FastoObject::FastoObject(const FastoObjectPtr &parent, const fastoType &type, const value_type *memory)
        : _parent(parent), _type(type), alloc_size_(strlen(memory)+1), size_(alloc_size_-1),
          memory_((value_type*)calloc(alloc_size_,sizeof(value_type)))
    {
        DCHECK(memory_);
        memcpy(memory_,memory,size_);
    }

    FastoObject::FastoObject(const FastoObjectPtr &parent, const fastoType &type, const value_type *memory, uint32_t size)
        : _parent(parent), _type(type), alloc_size_(size+1), size_(size),
          memory_((value_type*)calloc(alloc_size_,sizeof(value_type)))
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

    FastoObject::FastoObject()
        :_parent(NULL), _type(ROOT), alloc_size_(0), size_(0), memory_(NULL)
    {

    }

    FastoObjectPtr FastoObject::createRoot()
    {
        FastoObjectPtr result = new FastoObject;
        return result;
    }

    void FastoObject::addChildren(const FastoObjectPtr &child)
    {
        if(child){
            _childrens.push_back(child);
        }
    }

    std::string toStdString(const FastoObjectPtr &obj)
    {
        std::string result;
        if(obj){
            FastoObject::child_container_type childrens = obj->_childrens;
            for(FastoObject::child_container_type::const_iterator it = childrens.begin(); it != childrens.end(); ++it ){
                result += toStdString(*it);
            }
            if(obj->c_str()){
                result += obj->c_str();
            }
        }
        return result;
    }
}
