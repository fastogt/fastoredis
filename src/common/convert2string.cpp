#include "common/convert2string.h"

#include <stdlib.h>

namespace common
{
    unicode_string convert2string(const std::string& from)
    {
        return from;
    }

    unicode_string convert2string(const buffer_type& from)
    {
        return unicode_string((const unicode_char*)from.c_str(), from.length());
    }

    unicode_string convert2string(bool from)
    {
        if(from){
            return UTEXT("true");
        }
        else{
            return UTEXT("false");
        }
    }

    unicode_string convert2string(int8_t val)
    {
        unicode_char buffer[2] = {0};
        unicode_sprintf(buffer, "%c", val);
        return buffer;
    }

    unicode_string convert2string(uint8_t val)
    {
        unicode_char buffer[2] = {0};
        unicode_sprintf(buffer, "%c", val);
        return buffer;
    }

    unicode_string convert2string(int16_t val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%d", val);
        return buffer;
    }

    unicode_string convert2string(uint16_t val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%u", val);
        return buffer;
    }

    unicode_string convert2string(int32_t val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%d", val);
        return buffer;
    }

    unicode_string convert2string(uint32_t val)
    {
        unicode_char buffer[16] = {0};
        unicode_sprintf(buffer, "%u", val);
        return buffer;
    }

    unicode_string convert2string(int64_t val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%lld", val);
        return buffer;
    }

    unicode_string convert2string(uint64_t val)
    {
        unicode_char buffer[32] = {0};
        unicode_sprintf(buffer, "%lld", val);
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
    std::string convertfromString(const unicode_string& val)
    {
        return val;
    }

    template<>
    buffer_type convertfromString(const unicode_string& val)
    {
        return buffer_type((const byte_type*)val.c_str(), val.length());
    }

    template<>
    char convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    unsigned char convertfromString(const unicode_string& val)
    {
        return atoi(val.c_str());
    }

    template<>
    bool convertfromString(const unicode_string& val)
    {
        if(val == UTEXT("true")){
            return true;
        }
        else if(val == UTEXT("false")){
            return false;
        }

        uint8_t intVal = convertfromString<uint8_t>(val);
        if(intVal == 0){
            return false;
        }

        return true;
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
