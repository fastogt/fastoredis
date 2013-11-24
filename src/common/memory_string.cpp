#include "common/memory_string.h"
#include <string.h>
#include <stdlib.h>
namespace common
{
memory_string::memory_string(uint32_t size)
    :alloc_size_(size+1),size_(0),
      memory_((unicode_char*)calloc(alloc_size_,sizeof(unicode_char)))
{
    DCHECK(memory_);
}
memory_string::memory_string(const unicode_char* memory)
    :alloc_size_(unicode_strlen(memory)+1),size_(alloc_size_-1),
      memory_((unicode_char*)calloc(alloc_size_,sizeof(unicode_char)))
{
    DCHECK(memory_);
    memcpy(memory_,memory,size_);
}
memory_string::memory_string(const memory_string &other)
    :alloc_size_(other.alloc_size_),size_(other.size_),
      memory_((unicode_char*)calloc(alloc_size_,sizeof(unicode_char)))
{
    DCHECK(memory_);
    memcpy(memory_,other.memory_,size_);
}
memory_string& memory_string::operator=(const memory_string &other)
{
    alloc_size_ = other.size_;
    size_ = other.size_;
    free(memory_);
    memory_ = (unicode_char*)calloc(alloc_size_,sizeof(unicode_char));
    memcpy(memory_,other.memory_,size_);
    DCHECK(size_<=alloc_size_);
    return *this;
}
memory_string::~memory_string()
{
    free(memory_);
}
void memory_string::append(unicode_char c)
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
void memory_string::append(const unicode_char *ptr)
{
    return append(ptr,unicode_strlen(ptr));
}
void memory_string::append(const void *ptr,uint32_t size)
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
        memory_ = (unicode_char*)realloc(memory_, alloc_size_);        
        if(memory_)
        {
           memset(memory_+size_,0,alloc_size_-size_);
        }
    }
}
memory_string::operator unicode_string()const
{
    return unicode_string(memory_,size_);
}
}
