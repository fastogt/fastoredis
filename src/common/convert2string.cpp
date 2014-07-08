#include "common/convert2string.h"

#include <stdlib.h>

namespace common
{
    unicode_string convert2string(const std::string& from)
    {
        return from;
    }

    unicode_string convert2string(short val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%d", val);
        return buffer;
    }

    unicode_string convert2string(unsigned short val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%u", val);
        return buffer;
    }

    unicode_string convert2string(int val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%d", val);
        return buffer;
    }

    unicode_string convert2string(unsigned int val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%u", val);
        return buffer;
    }

    unicode_string convert2string(long val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%ld", val);
        return buffer;
    }

    unicode_string convert2string(unsigned long val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%uld", val);
        return buffer;
    }

    unicode_string convert2string(long long val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%lld", val);
        return buffer;
    }

    unicode_string convert2string(unsigned long long val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%ulld", val);
        return buffer;
    }

    unicode_string convert2string(float val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%f", val);
        return buffer;
    }

    unicode_string convert2string(double val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%f", val);
        return buffer;
    }

    template<>
    short convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    unsigned short convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    int convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    unsigned int convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    long convertfromString(const unicode_string& val)
    {
        return atoll(val.c_str());
    }

    template<>
    unsigned long convertfromString(const unicode_string& val)
    {
        return atoll(val.c_str());
    }

    template<>
    long long convertfromString(const unicode_string& val)
    {
        return atoll(val.c_str());
    }

    template<>
    unsigned long long convertfromString(const unicode_string& val)
    {
        return atoll(val.c_str());
    }

    template<>
    float convertfromString(const unicode_string& val)
    {
        return atof(val.c_str());
    }

    template<>
    double convertfromString(const unicode_string& val)
    {
        return atof(val.c_str());
    }
}
