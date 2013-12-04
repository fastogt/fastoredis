#pragma once

#include "common/macros.h"

namespace common
{
    class memory_string
    {
    public:
        typedef unsigned int uint32_t;
        explicit memory_string(uint32_t size);
        explicit memory_string(const unicode_char *memory);
        memory_string(const memory_string& other);
        memory_string& operator=(const memory_string &other);
        void append(unicode_char c);
        void append(const memory_string &src);
        void append(const unicode_char *ptr);
        void append(const void *ptr, uint32_t size);
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
        template<typename... targs_t>
        inline void append_format(const unicode_char* format,targs_t... args)
        {
            size_t len = unicode_strlen(format,args...)-(sizeof...(targs_t))*2;
            alloc(size_+len);
            int count_writen=ERROR_RESULT_VALUE;
            if((count_writen = unicode_sprintf(memory_+size_,format,args...))!=ERROR_RESULT_VALUE)
            {
                size_+=count_writen;
            }
            DCHECK(size_<alloc_size_);
        }
    #endif
        ~memory_string();
        uint32_t size() const
        {
            return size_;
        }
        uint32_t alloc_size() const
        {
            return alloc_size_;
        }
        const unicode_char * const c_str() const
        {
            return memory_;
        }
        const unicode_char *begin()const
        {
            return memory_;
        }
        const unicode_char *end()const
        {
            return memory_+size_;
        }
        operator unicode_string() const;
    private:
        void alloc(uint32_t strlen_result);
        uint32_t alloc_size_;
        uint32_t size_;
        unicode_char* memory_;
    };
}
