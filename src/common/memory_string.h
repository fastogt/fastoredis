#pragma once

#include "common/types.h"

namespace common
{
    class memory_string
    {
    public:
        explicit memory_string(uint32_t size);
        explicit memory_string(const char16 *memory);
        ~memory_string();

        memory_string(const memory_string& other);
        memory_string& operator=(const memory_string &other);

        void append(char16 c);
        void append(const memory_string &src);
        void append(const char16 *ptr);
        void append(const void *ptr, uint32_t size);

        uint32_t size() const;
        uint32_t alloc_size() const;
        const char16 * const c_str() const;
        const char16 *begin()const;
        const char16 *end()const;
        operator string16() const;

    private:
        void alloc(uint32_t strlen_result);
        uint32_t alloc_size_;
        uint32_t size_;
        char16* memory_;
    };
}
