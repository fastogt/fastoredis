#include "common/memory_string.h"

#include <string.h>
#include <stdlib.h>

namespace common
{
    memory_string::memory_string(uint32_t size)
        :alloc_size_(size+1),size_(0),
          memory_((char16*)calloc(alloc_size_,sizeof(char16)))
    {
        DCHECK(memory_);
    }

    memory_string::memory_string(const char16* memory)
        :alloc_size_(c16len(memory)+1),size_(alloc_size_-1),
          memory_((char16*)calloc(alloc_size_,sizeof(char16)))
    {
        DCHECK(memory_);
        memcpy(memory_,memory,size_);
    }

    memory_string::memory_string(const memory_string &other)
        :alloc_size_(other.alloc_size_),size_(other.size_),
          memory_((char16*)calloc(alloc_size_,sizeof(char16)))
    {
        DCHECK(memory_);
        memcpy(memory_,other.memory_,size_);
    }

    memory_string& memory_string::operator=(const memory_string &other)
    {
        alloc_size_ = other.size_;
        size_ = other.size_;
        free(memory_);
        memory_ = (char16*)calloc(alloc_size_,sizeof(char16));
        memcpy(memory_,other.memory_,size_);
        DCHECK(size_<=alloc_size_);
        return *this;
    }

    memory_string::~memory_string()
    {
        free(memory_);
    }

    void memory_string::append(char16 c)
    {
        alloc(size_ + 1);
        if(memory_)
        {
            memory_[size_++] = c;
        }
        DCHECK(size_<=alloc_size_);
    }

    void memory_string::append(const memory_string &src)
    {
        return append(src.memory_,src.size_);
    }

    void memory_string::append(const char16 *ptr)
    {
        return append(ptr, c16len(ptr));
    }

    void memory_string::append(const void *ptr, uint32_t size)
    {
        alloc(size_ + size);
        if (memory_&&ptr)
        {
          memcpy(memory_+size_, ptr, size);
          size_ += size;
        }
        DCHECK(size_<=alloc_size_);
    }

    void memory_string::alloc(uint32_t strlen_result)
    {
        if(alloc_size_<strlen_result+1)
        {
            alloc_size_=strlen_result*2+1;
            memory_ = (char16*)realloc(memory_, alloc_size_);
            if(memory_)
            {
               memset(memory_+size_,0,alloc_size_-size_);
            }
        }
    }

    memory_string::operator string16()const
    {
        return string16(memory_,size_);
    }

    uint32_t memory_string::size() const
    {
        return size_;
    }

    uint32_t memory_string::alloc_size() const
    {
        return alloc_size_;
    }

    const char16 * const memory_string::c_str() const
    {
        return memory_;
    }

    const char16 *memory_string::begin()const
    {
        return memory_;
    }

    const char16 *memory_string::end()const
    {
        return memory_+size_;
    }
}
