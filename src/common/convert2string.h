#pragma once

/**/

#include "common/types.h"

namespace common
{
    unicode_string convert2string(const std::string& from);
    unicode_string convert2string(const buffer_type& from);
    unicode_string convert2string(bool from);

    unicode_string convert2string(int8_t val);
    unicode_string convert2string(uint8_t val);
    unicode_string convert2string(int16_t val);
    unicode_string convert2string(uint16_t val);
    unicode_string convert2string(int32_t val);
    unicode_string convert2string(uint32_t val);
    unicode_string convert2string(int64_t val);
    unicode_string convert2string(uint64_t val);
    unicode_string convert2string(float val);
    unicode_string convert2string(double val);

    template<typename T>
    T convertfromString(const unicode_string& from);
}
