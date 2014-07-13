#pragma once

/**/

#include <vector>

namespace common
{
    namespace runtime_bitset
    {
        struct bit_set
        {
            typedef unsigned type;
            enum{ size_of_type = sizeof(type), count_bits_in_type = 8 * size_of_type};

            explicit bit_set(unsigned size);
            template<typename type_t>
            bit_set(unsigned size,const std::vector<type_t> &nodes);
            ~bit_set();

            void set_bit(unsigned idx);
            void clear_bit(unsigned idx);
            unsigned get_bit(unsigned idx) const;
            unsigned get_bit_in_ancor(float first_ancor,float last_ancor)const;
            bool get_first_checked_bit(unsigned idx,unsigned &ret)const;
            bool get_next_checked_bit(unsigned idx,unsigned &ret)const;
            bool get_previos_checked_bit(unsigned idx,unsigned &ret)const;
            unsigned get_count_checked_bit(unsigned offset) const;
            bool get_n_checked_bit(unsigned n,unsigned &out) const;
            void print() const;

            unsigned size()const;

        private:
            const unsigned size_;
            type*const data_;
        };
    }
}
