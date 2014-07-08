#pragma once

#include "common/types.h"

namespace common
{
    class memory_string
    {
    public:
        explicit memory_string(uint32_type size);
        explicit memory_string(const unicode_char *memory);
        ~memory_string();

        memory_string(const memory_string& other);
        memory_string& operator=(const memory_string &other);

        void append(unicode_char c);
        void append(const memory_string &src);
        void append(const unicode_char *ptr);
        void append(const void *ptr, uint32_type size);

        uint32_type size() const;
        uint32_type alloc_size() const;
        const unicode_char * const c_str() const;
        const unicode_char *begin()const;
        const unicode_char *end()const;
        operator unicode_string() const;

    private:
        void alloc(uint32_type strlen_result);
        uint32_type alloc_size_;
        uint32_type size_;
        unicode_char* memory_;
    };
}
