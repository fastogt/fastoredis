#pragma once

/**/

#include "common/types.h"

namespace common
{
    unicode_string convert2string(const std::string& from);

    unicode_string convert2string(short val);
    unicode_string convert2string(unsigned short val);
    unicode_string convert2string(int val);
    unicode_string convert2string(unsigned int val);
    unicode_string convert2string(long val);
    unicode_string convert2string(unsigned long val);
    unicode_string convert2string(long long val);
    unicode_string convert2string(unsigned long long val);
    unicode_string convert2string(float val);
    unicode_string convert2string(double val);

    template<typename T>
    T convertfromString(const unicode_string& from);
}
