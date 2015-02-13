#include "common/memory_string.h"

#include <string.h>
#include <stdlib.h>

namespace common
{
    memory_string::memory_string(uint32_t size)
        :alloc_size_(size+1),size_(0),
          memory_((char*)calloc(alloc_size_,sizeof(char)))
    {
        DCHECK(memory_);
    }

    memory_string::memory_string(const char* memory)
        :alloc_size_(strlen(memory)+1),size_(alloc_size_-1),
          memory_((char*)calloc(alloc_size_,sizeof(char)))
    {
        DCHECK(memory_);
        memcpy(memory_,memory,size_);
    }

    memory_string::memory_string(const memory_string& other)
        :alloc_size_(other.alloc_size_),size_(other.size_),
          memory_((char*)calloc(alloc_size_,sizeof(char)))
    {
        DCHECK(memory_);
        memcpy(memory_,other.memory_,size_);
    }

    memory_string& memory_string::operator=(const memory_string& other)
    {
        alloc_size_ = other.size_;
        size_ = other.size_;
        free(memory_);
        memory_ = (char*)calloc(alloc_size_,sizeof(char));
        memcpy(memory_,other.memory_,size_);
        DCHECK(size_<=alloc_size_);
        return *this;
    }

    memory_string::~memory_string()
    {
        free(memory_);
    }

    void memory_string::append(char c)
    {
        alloc(size_ + 1);
        if(memory_)
        {
            memory_[size_++] = c;
        }
        DCHECK(size_<=alloc_size_);
    }

    void memory_string::append(const memory_string& src)
    {
        return append(src.memory_,src.size_);
    }

    void memory_string::append(const char *ptr)
    {
        return append(ptr, strlen(ptr));
    }

    void memory_string::append(const void* ptr, uint32_t size)
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
            memory_ = (char*)realloc(memory_, alloc_size_);
            if(memory_)
            {
               memset(memory_+size_,0,alloc_size_-size_);
            }
        }
    }

    memory_string::operator std::string()const
    {
        return std::string(memory_,size_);
    }

    uint32_t memory_string::size() const
    {
        return size_;
    }

    uint32_t memory_string::alloc_size() const
    {
        return alloc_size_;
    }

    const char * const memory_string::c_str() const
    {
        return memory_;
    }

    const char *memory_string::begin()const
    {
        return memory_;
    }

    const char *memory_string::end()const
    {
        return memory_+size_;
    }
}
