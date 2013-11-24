#include "common/runtime_bitset.h"
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include "common/macros.h"
namespace common
{
namespace runtime_bitset
{
    template<>
    bit_set::bit_set(unsigned size, const std::vector<unsigned> &nodes)
        :size_(size),data_((type*)malloc(size_ / size_of_type + 1))
    {
        VERIFY(0<size_);
        memset(data_,0,size_ / size_of_type + 1);
        for(unsigned i = 0;i<nodes.size();i+=2)
        {
            unsigned last = nodes[i+1];
            for(unsigned first = nodes[i];first < last;++first )
            {
                set_bit(first);
            }
        }
    }
    bit_set::bit_set(unsigned size):size_(size),data_((type*)malloc(size_ / size_of_type + 1))
    {
        memset(data_,0,size_ / size_of_type + 1);
    }
    void bit_set::set_bit(unsigned idx)
    {
        data_[idx / count_bits_in_type] |= (1 << (idx % count_bits_in_type));
    }
    void bit_set::clear_bit(unsigned idx)
    {
        data_[idx / count_bits_in_type] &= (0 << (idx % count_bits_in_type));
    }
    unsigned bit_set::get_bit(unsigned idx) const
    {
        return (data_[idx / count_bits_in_type] >>idx )&1 ;
    }
    unsigned bit_set::get_bit_in_ancor(float first_ancor,float last_ancor)const
    {
        return get_bit((unsigned)first_ancor)&&get_bit((unsigned)last_ancor);
    }
    bool bit_set::get_first_checked_bit(unsigned idx,unsigned &ret)const
    {
        bool result = false;
        for (unsigned i =idx; i < size_;++i)
        {
            if(get_bit(i))
            {
                ret = i;
                result = true;
                break;
            }
        }
        return result;
    }
    unsigned bit_set::get_count_checked_bit(unsigned offset) const
    {
        unsigned result = 0;
        for(unsigned i=0;i<offset;++i)
        {
            result+=get_bit(i);
        }
        return result;
    }
    bool bit_set::get_n_checked_bit(unsigned n,unsigned &out) const
    {
        bool result = false;
        out = 0;
        for(unsigned i=0;i<size_;++i)
        {
            out+=get_bit(i);
            if(out==n)
            {
                out=i;
                result = true;
                break;
            }
        }
        return result;
    }
    bool bit_set::get_next_checked_bit(unsigned idx,unsigned &ret)const
    {
        return get_first_checked_bit(idx+1,ret);
    }
    bool bit_set::get_previos_checked_bit(unsigned idx,unsigned &ret)const
    {
        bool result = false;
        for (unsigned i =0; i < idx;++i)
        {
            if(get_bit(i))
            {
                ret = i;
                result = true;
                break;
            }
        }
        return result;
    }
    void bit_set::print() const
    {
        for (unsigned i =0; i < size_;++i)
        {
            printf("%d %d\n",i,get_bit(i));
        }
    }
    bit_set::~bit_set()
    {
        free(data_);
    }
    unsigned bit_set::size()const
    {
        return size_;
    }
}
}
