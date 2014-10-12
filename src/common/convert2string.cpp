#include "common/convert2string.h"

#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>

#include <limits>
#include <vector>

#include "common/string_piece.h"

namespace common
{
    namespace
    {

    template <typename STR, typename INT, typename UINT, bool NEG>
    struct IntToStringT {
      // This is to avoid a compiler warning about unary minus on unsigned type.
      // For example, say you had the following code:
      //   template <typename INT>
      //   INT abs(INT value) { return value < 0 ? -value : value; }
      // Even though if INT is unsigned, it's impossible for value < 0, so the
      // unary minus will never be taken, the compiler will still generate a
      // warning.  We do a little specialization dance...
      template <typename INT2, typename UINT2, bool NEG2>
      struct ToUnsignedT {};

      template <typename INT2, typename UINT2>
      struct ToUnsignedT<INT2, UINT2, false> {
        static UINT2 ToUnsigned(INT2 value) {
          return static_cast<UINT2>(value);
        }
      };

      template <typename INT2, typename UINT2>
      struct ToUnsignedT<INT2, UINT2, true> {
        static UINT2 ToUnsigned(INT2 value) {
          return static_cast<UINT2>(value < 0 ? -value : value);
        }
      };

      // This set of templates is very similar to the above templates, but
      // for testing whether an integer is negative.
      template <typename INT2, bool NEG2>
      struct TestNegT {};
      template <typename INT2>
      struct TestNegT<INT2, false> {
        static bool TestNeg(INT2 value) {
          // value is unsigned, and can never be negative.
          return false;
        }
      };
      template <typename INT2>
      struct TestNegT<INT2, true> {
        static bool TestNeg(INT2 value) {
          return value < 0;
        }
      };

      static STR IntToString(INT value) {
        // log10(2) ~= 0.3 bytes needed per bit or per byte log10(2**8) ~= 2.4.
        // So round up to allocate 3 output characters per byte, plus 1 for '-'.
        const int kOutputBufSize = 3 * sizeof(INT) + 1;

        // Allocate the whole string right away, we will right back to front, and
        // then return the substr of what we ended up using.
        STR outbuf(kOutputBufSize, 0);

        bool is_neg = TestNegT<INT, NEG>::TestNeg(value);
        // Even though is_neg will never be true when INT is parameterized as
        // unsigned, even the presence of the unary operation causes a warning.
        UINT res = ToUnsignedT<INT, UINT, NEG>::ToUnsigned(value);

        typename STR::iterator it(outbuf.end());
        do {
          --it;
          DCHECK(it != outbuf.begin());
          *it = static_cast<typename STR::value_type>((res % 10) + '0');
          res /= 10;
        } while (res != 0);
        if (is_neg) {
          --it;
          DCHECK(it != outbuf.begin());
          *it = static_cast<typename STR::value_type>('-');
        }
        return STR(it, outbuf.end());
      }
    };

    // Utility to convert a character to a digit in a given base
    template<typename CHAR, int BASE, bool BASE_LTE_10> class BaseCharToDigit {
    };

    // Faster specialization for bases <= 10
    template<typename CHAR, int BASE> class BaseCharToDigit<CHAR, BASE, true> {
     public:
      static bool Convert(CHAR c, uint8_t* digit) {
        if (c >= '0' && c < '0' + BASE) {
          *digit = c - '0';
          return true;
        }
        return false;
      }
    };

    // Specialization for bases where 10 < base <= 36
    template<typename CHAR, int BASE> class BaseCharToDigit<CHAR, BASE, false> {
     public:
      static bool Convert(CHAR c, uint8_t* digit) {
        if (c >= '0' && c <= '9') {
          *digit = c - '0';
        } else if (c >= 'a' && c < 'a' + BASE - 10) {
          *digit = c - 'a' + 10;
        } else if (c >= 'A' && c < 'A' + BASE - 10) {
          *digit = c - 'A' + 10;
        } else {
          return false;
        }
        return true;
      }
    };

    template<int BASE, typename CHAR> bool CharToDigit(CHAR c, uint8_t* digit) {
      return BaseCharToDigit<CHAR, BASE, BASE <= 10>::Convert(c, digit);
    }

    // There is an IsWhitespace for wchars defined in string_util.h, but it is
    // locale independent, whereas the functions we are replacing were
    // locale-dependent. TBD what is desired, but for the moment let's not introduce
    // a change in behaviour.
    template<typename CHAR> class WhitespaceHelper {
    };

    template<> class WhitespaceHelper<char> {
     public:
      static bool Invoke(char c) {
        return 0 != isspace(static_cast<unsigned char>(c));
      }
    };

    template<> class WhitespaceHelper<char16> {
     public:
      static bool Invoke(char16 c) {
        return 0 != iswspace(c);
      }
    };

    template<typename CHAR> bool LocalIsWhitespace(CHAR c) {
      return WhitespaceHelper<CHAR>::Invoke(c);
    }

    // IteratorRangeToNumberTraits should provide:
    //  - a typedef for iterator_type, the iterator type used as input.
    //  - a typedef for value_type, the target numeric type.
    //  - static functions min, max (returning the minimum and maximum permitted
    //    values)
    //  - constant kBase, the base in which to interpret the input
    template<typename IteratorRangeToNumberTraits>
    class IteratorRangeToNumber {
     public:
      typedef IteratorRangeToNumberTraits traits;
      typedef typename traits::iterator_type const_iterator;
      typedef typename traits::value_type value_type;

      // Generalized iterator-range-to-number conversion.
      //
      static bool Invoke(const_iterator begin,
                         const_iterator end,
                         value_type* output) {
        bool valid = true;

        while (begin != end && LocalIsWhitespace(*begin)) {
          valid = false;
          ++begin;
        }

        if (begin != end && *begin == '-') {
          if (!std::numeric_limits<value_type>::is_signed) {
            valid = false;
          } else if (!Negative::Invoke(begin + 1, end, output)) {
            valid = false;
          }
        } else {
          if (begin != end && *begin == '+') {
            ++begin;
          }
          if (!Positive::Invoke(begin, end, output)) {
            valid = false;
          }
        }

        return valid;
      }

     private:
      // Sign provides:
      //  - a static function, CheckBounds, that determines whether the next digit
      //    causes an overflow/underflow
      //  - a static function, Increment, that appends the next digit appropriately
      //    according to the sign of the number being parsed.
      template<typename Sign>
      class Base {
       public:
        static bool Invoke(const_iterator begin, const_iterator end,
                           typename traits::value_type* output) {
          *output = 0;

          if (begin == end) {
            return false;
          }

          // Note: no performance difference was found when using template
          // specialization to remove this check in bases other than 16
          if (traits::kBase == 16 && end - begin > 2 && *begin == '0' &&
              (*(begin + 1) == 'x' || *(begin + 1) == 'X')) {
            begin += 2;
          }

          for (const_iterator current = begin; current != end; ++current) {
            uint8_t new_digit = 0;

            if (!CharToDigit<traits::kBase>(*current, &new_digit)) {
              return false;
            }

            if (current != begin) {
              if (!Sign::CheckBounds(output, new_digit)) {
                return false;
              }
              *output *= traits::kBase;
            }

            Sign::Increment(new_digit, output);
          }
          return true;
        }
      };

      class Positive : public Base<Positive> {
       public:
        static bool CheckBounds(value_type* output, uint8_t new_digit) {
          if (*output > static_cast<value_type>(traits::max() / traits::kBase) ||
              (*output == static_cast<value_type>(traits::max() / traits::kBase) &&
               new_digit > traits::max() % traits::kBase)) {
            *output = traits::max();
            return false;
          }
          return true;
        }
        static void Increment(uint8_t increment, value_type* output) {
          *output += increment;
        }
      };

      class Negative : public Base<Negative> {
       public:
        static bool CheckBounds(value_type* output, uint8_t new_digit) {
          if (*output < traits::min() / traits::kBase ||
              (*output == traits::min() / traits::kBase &&
               new_digit > 0 - traits::min() % traits::kBase)) {
            *output = traits::min();
            return false;
          }
          return true;
        }
        static void Increment(uint8_t increment, value_type* output) {
          *output -= increment;
        }
      };
    };

    template<typename ITERATOR, typename VALUE, int BASE>
    class BaseIteratorRangeToNumberTraits {
     public:
      typedef ITERATOR iterator_type;
      typedef VALUE value_type;
      static value_type min() {
        return std::numeric_limits<value_type>::min();
      }
      static value_type max() {
        return std::numeric_limits<value_type>::max();
      }
      static const int kBase = BASE;
    };

    template<typename ITERATOR>
    class BaseHexIteratorRangeToIntTraits
        : public BaseIteratorRangeToNumberTraits<ITERATOR, int, 16> {
    };

    template<typename ITERATOR>
    class BaseHexIteratorRangeToUIntTraits
        : public BaseIteratorRangeToNumberTraits<ITERATOR, uint32_t, 16> {
    };

    template<typename ITERATOR>
    class BaseHexIteratorRangeToInt64Traits
        : public BaseIteratorRangeToNumberTraits<ITERATOR, int64_t, 16> {
    };

    template<typename ITERATOR>
    class BaseHexIteratorRangeToUInt64Traits
        : public BaseIteratorRangeToNumberTraits<ITERATOR, uint64_t, 16> {
    };

    typedef BaseHexIteratorRangeToIntTraits<StringPiece::const_iterator>
        HexIteratorRangeToIntTraits;

    typedef BaseHexIteratorRangeToUIntTraits<StringPiece::const_iterator>
        HexIteratorRangeToUIntTraits;

    typedef BaseHexIteratorRangeToInt64Traits<StringPiece::const_iterator>
        HexIteratorRangeToInt64Traits;

    typedef BaseHexIteratorRangeToUInt64Traits<StringPiece::const_iterator>
        HexIteratorRangeToUInt64Traits;

    template<typename STR>
    bool HexStringToBytesT(const STR& input, std::vector<uint8_t>* output) {
      DCHECK_EQ(output->size(), 0u);
      size_t count = input.size();
      if (count == 0 || (count % 2) != 0)
        return false;
      for (uintptr_t i = 0; i < count / 2; ++i) {
        uint8_t msb = 0;  // most significant 4 bits
        uint8_t lsb = 0;  // least significant 4 bits
        if (!CharToDigit<16>(input[i * 2], &msb) ||
            !CharToDigit<16>(input[i * 2 + 1], &lsb))
          return false;
        output->push_back((msb << 4) | lsb);
      }
      return true;
    }

    template <typename VALUE, int BASE>
    class StringPieceToNumberTraits
        : public BaseIteratorRangeToNumberTraits<StringPiece::const_iterator,
                                                 VALUE,
                                                 BASE> {
    };

    template <typename VALUE>
    bool StringToIntImpl(const StringPiece& input, VALUE* output) {
      return IteratorRangeToNumber<StringPieceToNumberTraits<VALUE, 10> >::Invoke(
          input.begin(), input.end(), output);
    }

    template <typename VALUE, int BASE>
    class StringPiece16ToNumberTraits
        : public BaseIteratorRangeToNumberTraits<StringPiece16::const_iterator,
                                                 VALUE,
                                                 BASE> {
    };

    template <typename VALUE>
    bool String16ToIntImpl(const StringPiece16& input, VALUE* output) {
      return IteratorRangeToNumber<StringPiece16ToNumberTraits<VALUE, 10> >::Invoke(
          input.begin(), input.end(), output);
    }

    }  // namespace

    string16 convertToString16(const std::string& from)
    {
        return string16((const char16*)from.c_str(), from.length());
    }

    string16 convertToString16(const buffer_type& from)
    {
        return string16((const char16*)from.c_str(), from.length());
    }

    string16 convertToString16(bool from)
    {
        if(from){
            return UTEXT("true");
        }
        else{
            return UTEXT("false");
        }
    }

    string16 convertToString16(char value)
    {
        return IntToStringT<string16, char, unsigned char, true>::
              IntToString(value);
    }

    string16 convertToString16(unsigned char value)
    {
        return IntToStringT<string16, char, unsigned char, false>::
              IntToString(value);
    }

    string16 convertToString16(short value)
    {
        return IntToStringT<string16, short, unsigned short, true>::
              IntToString(value);
    }

    string16 convertToString16(unsigned short value)
    {
        return IntToStringT<string16, short, unsigned short, false>::
              IntToString(value);
    }

    string16 convertToString16(int value)
    {
        return IntToStringT<string16, int, unsigned int, true>::
              IntToString(value);
    }

    string16 convertToString16(unsigned int value)
    {
        return IntToStringT<string16, int, unsigned int, false>::
              IntToString(value);
    }

    string16 convertToString16(long value)
    {
        return IntToStringT<string16, long, unsigned long, true>::
              IntToString(value);
    }

    string16 convertToString16(unsigned long value)
    {
        return IntToStringT<string16, long, unsigned long, false>::
              IntToString(value);
    }

    string16 convertToString16(long long value)
    {
        return IntToStringT<string16, long long, unsigned long long, true>::
              IntToString(value);
    }

    string16 convertToString16(unsigned long long value)
    {
        return IntToStringT<string16, long long, unsigned long long, false>::
              IntToString(value);
    }

    string16 convertToString16(float value)
    {
        int val = value;
        return convertToString16(val);
    }

    string16 convertToString16(double value)
    {
        int val = value;
        return convertToString16(val);
    }
//
    template<>
    std::string convertFromString16(const string16& input)
    {
        return std::string(input.begin(), input.end());
    }

    template<>
    buffer_type convertFromString16(const string16& val)
    {
        return buffer_type((const byte_type*)val.c_str(), val.length());
    }

    template<>
    char convertFromString16(const string16& input)
    {
        char output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned char convertFromString16(const string16& input)
    {
        unsigned char output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    bool convertFromString16(const string16& val)
    {
        if(val == UTEXT("true")){
            return true;
        }
        else if(val == UTEXT("false")){
            return false;
        }

        uint8_t intVal = convertFromString16<uint8_t>(val);
        if(intVal == 0){
            return false;
        }

        return true;
    }

    template<>
    short convertFromString16(const string16& input)
    {
        short output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned short convertFromString16(const string16& input)
    {
        unsigned short output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    int convertFromString16(const string16& input)
    {
        int output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned int convertFromString16(const string16& input)
    {
        unsigned int output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    long convertFromString16(const string16& input)
    {
        long output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned long convertFromString16(const string16& input)
    {
        unsigned long output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    long long convertFromString16(const string16& input)
    {
        long long output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned long long convertFromString16(const string16& input)
    {
        unsigned long long output = 0;
        bool res = String16ToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    float convertFromString16(const string16& val)
    {
        return convertFromString16<int>(val);
    }

    template<>
    double convertFromString16(const string16& val)
    {
        return convertFromString16<int>(val);
    }

// std string

    std::string convertToString(const buffer_type& from)
    {
        return std::string((const char*)from.c_str(), from.length());
    }

    std::string convertToString(const string16& from)
    {
        return convertFromString16<std::string>(from);
    }

    std::string convertToString(bool from)
    {
        if(from){
            return "true";
        }
        else{
            return "false";
        }
    }

    std::string convertToString(char value)
    {
        return IntToStringT<std::string, char, unsigned char, true>::
              IntToString(value);
    }

    std::string convertToString(unsigned char value)
    {
        return IntToStringT<std::string, char, unsigned char, false>::
              IntToString(value);
    }

    std::string convertToString(short value)
    {
        return IntToStringT<std::string, short, unsigned short, true>::
              IntToString(value);
    }

    std::string convertToString(unsigned short value)
    {
        return IntToStringT<std::string, short, unsigned short, false>::
              IntToString(value);
    }

    std::string convertToString(int value)
    {
        return IntToStringT<std::string, int, unsigned int, true>::
              IntToString(value);
    }

    std::string convertToString(unsigned int value)
    {
        return IntToStringT<std::string, int, unsigned int, false>::
              IntToString(value);
    }

    std::string convertToString(long value)
    {
        return IntToStringT<std::string, long, unsigned long, true>::
              IntToString(value);
    }

    std::string convertToString(unsigned long value)
    {
        return IntToStringT<std::string, long, unsigned long, false>::
              IntToString(value);
    }

    std::string convertToString(long long value)
    {
        return IntToStringT<std::string, long long, unsigned long long, false>::
              IntToString(value);
    }

    std::string convertToString(unsigned long long value)
    {
        return IntToStringT<std::string, long long, unsigned long long, false>::
              IntToString(value);
    }

    std::string convertToString(float value)
    {
        int val = value;
        return convertToString(val);
    }

    std::string convertToString(double value)
    {
        int val = value;
        return convertToString(val);
    }
//
    template<>
    buffer_type convertFromString(const std::string& val)
    {
        return buffer_type((const byte_type*)val.c_str(), val.length());
    }

    template<>
    string16 convertFromString(const std::string& input)
    {
        return convertToString16(input);
    }

    template<>
    char convertFromString(const std::string& input)
    {
        char output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned char convertFromString(const std::string& input)
    {
        unsigned char output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    bool convertFromString(const std::string& val)
    {
        if(val == "true"){
            return true;
        }
        else if(val == "false"){
            return false;
        }

        uint8_t intVal = convertFromString<uint8_t>(val);
        if(intVal == 0){
            return false;
        }

        return true;
    }

    template<>
    short convertFromString(const std::string& input)
    {
        short output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned short convertFromString(const std::string& input)
    {
        unsigned short output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    int convertFromString(const std::string& input)
    {
        int output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned int convertFromString(const std::string& input)
    {
        unsigned int output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    long convertFromString(const std::string& input)
    {
        long output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned long convertFromString(const std::string& input)
    {
        unsigned long output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    long long convertFromString(const std::string& input)
    {
        long long output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    unsigned long long convertFromString(const std::string& input)
    {
        unsigned long long output = 0;
        bool res = StringToIntImpl(input, &output);
        DCHECK(res);
        return output;
    }

    template<>
    float convertFromString(const std::string& val)
    {
        return convertFromString<int>(val);
    }

    template<>
    double convertFromString(const std::string& val)
    {
        return convertFromString<int>(val);
    }
}
