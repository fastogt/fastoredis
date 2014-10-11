#pragma once

/**/

#include "common/types.h"

namespace common
{
    string16 convertToString16(const std::string& from);
    string16 convertToString16(const buffer_type& from);
    string16 convertToString16(bool from);

    string16 convertToString16(char value);
    string16 convertToString16(unsigned char value);
    string16 convertToString16(short value);
    string16 convertToString16(unsigned short value);
    string16 convertToString16(int value);
    string16 convertToString16(unsigned int value);
    string16 convertToString16(long value);
    string16 convertToString16(unsigned long value);
    string16 convertToString16(long long value);
    string16 convertToString16(unsigned long long value);
    string16 convertToString16(float value);
    string16 convertToString16(double value);

// std::string

    std::string convertToString(const buffer_type& from);
    std::string convertToString(const string16& from);
    std::string convertToString(bool from);

    std::string convertToString(char value);
    std::string convertToString(unsigned char value);
    std::string convertToString(short value);
    std::string convertToString(unsigned short value);
    std::string convertToString(int value);
    std::string convertToString(unsigned int value);
    std::string convertToString(long value);
    std::string convertToString(unsigned long value);
    std::string convertToString(long long value);
    std::string convertToString(unsigned long long value);
    std::string convertToString(float value);
    std::string convertToString(double value);

    template<typename T>
    T convertFromString16(const string16& from);

    template<typename T>
    T convertFromString(const std::string& from);
}
