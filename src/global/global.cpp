#include "global/global.h"

#include <string.h>
#include <stdlib.h>

#include "common/macros.h"

namespace fastoredis
{
    FastoObject::FastoObject(const fastoType &type, const value_type *memory)
        :_type(type), size_(strlen(memory)),
          memory_((value_type*)calloc(size_+1,sizeof(value_type)))
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

    FastoObject::~FastoObject()
    {
        free(memory_);
    }

}
