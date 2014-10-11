#pragma once

#include <string>

#include "common/string_piece.h"

namespace common {

// These convert between UTF-8, -16, and -32 strings. They are potentially slow,
// so avoid unnecessary conversions. The low-level versions return a boolean
// indicating whether the conversion was 100% valid. In this case, it will still
// do the best it can and put the result in the output buffer. The versions that
// return strings ignore this error and just return the best conversion
// possible.
bool WideToUTF8(const wchar_t* src, size_t src_len,
                            std::string* output);
std::string WideToUTF8(const std::wstring& wide);
bool UTF8ToWide(const char* src, size_t src_len,
                            std::wstring* output);
std::wstring UTF8ToWide(const StringPiece& utf8);

bool WideToUTF16(const wchar_t* src, size_t src_len,
                             string16* output);
string16 WideToUTF16(const std::wstring& wide);
bool UTF16ToWide(const char16* src, size_t src_len,
                             std::wstring* output);
std::wstring UTF16ToWide(const string16& utf16);

bool UTF8ToUTF16(const char* src, size_t src_len, string16* output);
string16 UTF8ToUTF16(const StringPiece& utf8);
bool UTF16ToUTF8(const char16* src, size_t src_len,
                             std::string* output);
std::string UTF16ToUTF8(const string16& utf16);

// These convert an ASCII string, typically a hardcoded constant, to a
// UTF16/Wide string.
std::wstring ASCIIToWide(const StringPiece& ascii);
string16 ASCIIToUTF16(const StringPiece& ascii);

// Converts to 7-bit ASCII by truncating. The result must be known to be ASCII
// beforehand.
std::string UTF16ToASCII(const string16& utf16);

}  // namespace base
