#pragma once

#include "common/types.h"

namespace common
{
    class memory_string
    {
    public:
        explicit memory_string(uint32_t size);
        explicit memory_string(const char *memory);
        ~memory_string();

        memory_string(const memory_string& other);
        memory_string& operator=(const memory_string &other);

        void append(char c);
        void append(const memory_string &src);
        void append(const char *ptr);
        void append(const void *ptr, uint32_t size);

        uint32_t size() const;
        uint32_t alloc_size() const;
        const char * const c_str() const;
        const char *begin()const;
        const char *end()const;
        operator std::string() const;

    private:
        void alloc(uint32_t strlen_result);
        uint32_t alloc_size_;
        uint32_t size_;
        char* memory_;
    };
}
