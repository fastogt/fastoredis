#pragma once

#include <vector>

#include "common/types.h"

#include "common/string_piece.h"

namespace common
{
    string16 convertToString16(const std::string& from);
    string16 convertToString16(const buffer_type& from);

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

    std::string HexEncode(const void* bytes, size_t size);

    bool HexStringToInt(const StringPiece& input, int* output);
    bool HexStringToInt64(const StringPiece& input, int64_t* output);
    bool HexStringToUInt64(const StringPiece& input, uint64_t *output);
    bool HexStringToBytes(const std::string& input, std::vector<uint8_t>* output);
}
