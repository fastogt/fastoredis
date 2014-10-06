#pragma once

/**/

#include <boost/scoped_ptr.hpp>
#include <vector>

#include "common/log_levels.h"

namespace common 
{
    class FundamentalValue;
    class ArrayValue;
    class StringValue;
    class ErrorValue;
    class Value;

    typedef std::vector<Value*> ValueVector;

    class Value
    {
    public:
        enum Type {
            TYPE_NULL = 0,
            TYPE_BOOLEAN,
            TYPE_INTEGER,
            TYPE_UINTEGER,
            TYPE_DOUBLE,
            TYPE_STRING,
            TYPE_ARRAY,
            TYPE_STATUS,
			TYPE_ERROR
        };

        enum ErrorsType { E_NONE, E_EXCEPTION, E_ERROR, E_INTERRUPTED };

        virtual ~Value();

        static Value* createNullValue();

        static FundamentalValue* createBooleanValue(bool in_value);
        static FundamentalValue* createIntegerValue(int in_value);
        static FundamentalValue* createDoubleValue(double in_value);
        static StringValue* createStringValue(const string16& in_value);
        static StringValue* createStringValue(const std::string& in_value);
        static ArrayValue* createArrayValue();
        static ErrorValue* createErrorValue(const string16& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level);
        static ErrorValue* createErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level);

        static string16 toString(Type t);
        virtual string16 toString() const;
        Type getType() const { return type_; }

        bool isType(Type type) const { return type == type_; }

        virtual bool getAsBoolean(bool* out_value) const;        
        virtual bool getAsInteger(int* out_value) const;
        virtual bool getAsUInteger(unsigned int* out_value) const;
        virtual bool getAsDouble(double* out_value) const;
        virtual bool getAsString(string16* out_value) const;
        virtual bool getAsError(ErrorValue* out_value) const;
        virtual bool getAsList(ArrayValue** out_value);
        virtual bool getAsList(const ArrayValue** out_value) const;

        virtual Value* deepCopy() const;

        virtual bool equals(const Value* other) const;

        static bool equals(const Value* a, const Value* b);

    protected:
        explicit Value(Type type);
        Value(const Value& that);
        Value& operator=(const Value& that);

    private:
        Type type_;
    };

    class FundamentalValue : public Value
    {
    public:
        explicit FundamentalValue(bool in_value);
        explicit FundamentalValue(int in_value);
        explicit FundamentalValue(unsigned int in_value);
        explicit FundamentalValue(double in_value);

        virtual ~FundamentalValue();

        virtual string16 toString() const;
        virtual bool getAsBoolean(bool* out_value) const;
        virtual bool getAsInteger(int* out_value) const;
        virtual bool getAsUInteger(unsigned int* out_value) const;
        virtual bool getAsDouble(double* out_value) const;
        virtual FundamentalValue* deepCopy() const;
        virtual bool equals(const Value* other) const;

    private:
		DISALLOW_COPY_AND_ASSIGN(FundamentalValue);
        union {
        bool boolean_value_;
        int integer_value_;
        unsigned int uinteger_value_;
        double double_value_;
        };
    };

    class StringValue : public Value
    {
        public:
            explicit StringValue(const std::string& in_value);
            explicit StringValue(const string16& in_value);
            virtual ~StringValue();

            virtual string16 toString() const;
            virtual bool getAsString(string16* out_value) const;
            virtual StringValue* deepCopy() const;
            virtual bool equals(const Value* other) const;

        private:
			DISALLOW_COPY_AND_ASSIGN(StringValue);
            string16 value_;
    };

    class ArrayValue : public Value
    {
    public:
        typedef ValueVector::iterator iterator;
        typedef ValueVector::const_iterator const_iterator;

        ArrayValue();
        virtual ~ArrayValue();

        virtual string16 toString() const;
        void clear();

        size_t getSize() const { return list_.size(); }

        // Returns whether the list is empty.
        bool empty() const { return list_.empty(); }

        bool set(size_t index, Value* in_value);

        bool get(size_t index, const Value** out_value) const;
        bool get(size_t index, Value** out_value);

        bool getBoolean(size_t index, bool* out_value) const;
        bool getInteger(size_t index, int* out_value) const;
        bool getDouble(size_t index, double* out_value) const;
        bool getString(size_t index, string16* out_value) const;
        bool getList(size_t index, const ArrayValue** out_value) const;
        bool getList(size_t index, ArrayValue** out_value);

        virtual bool remove(size_t index, boost::scoped_ptr<Value>* out_value);

        bool remove(const Value& value, size_t* index);

        iterator erase(iterator iter, boost::scoped_ptr<Value>* out_value);

        // Appends a Value to the end of the list.
        void append(Value* in_value);

        // Convenience forms of Append.
        void appendBoolean(bool in_value);
        void appendInteger(int in_value);
        void appendDouble(double in_value);
        void appendString(const string16& in_value);
        void appendStrings(const std::vector<string16> &in_values);

        bool appendIfNotPresent(Value* in_value);

        bool insert(size_t index, Value* in_value);

        const_iterator find(const Value& value) const;

        // Swaps contents with the |other| list.
        virtual void swap(ArrayValue* other);

        // Iteration.
        iterator begin() { return list_.begin(); }
        iterator end() { return list_.end(); }

        const_iterator begin() const { return list_.begin(); }
        const_iterator end() const { return list_.end(); }

        virtual bool getAsList(ArrayValue** out_value);
        virtual bool getAsList(const ArrayValue** out_value) const;
        virtual ArrayValue* deepCopy() const;
        virtual bool equals(const Value* other) const;

    private:
		DISALLOW_COPY_AND_ASSIGN(ArrayValue);
        ValueVector list_;
    };

    class ErrorValue : public Value
    {
    public:
        ErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level = common::logging::L_WARNING);
        ErrorValue(const string16& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level = common::logging::L_WARNING);
        ErrorValue();
        bool isError() const;
        common::logging::LEVEL_LOG level() const;
        string16 description() const;
        virtual string16 toString() const;

        virtual bool getAsError(ErrorValue* out_value) const;
        virtual ~ErrorValue();

    private:
		//DISALLOW_COPY_AND_ASSIGN(ErrorValue);
        string16 description_;
        ErrorsType errorType_;
        common::logging::LEVEL_LOG level_;
    };

    std::ostream& operator<<(std::ostream& out, const Value& value);

    inline std::ostream& operator<<(std::ostream& out, const FundamentalValue& value) {
        return out << static_cast<const Value&>(value);
    }

    inline std::ostream& operator<<(std::ostream& out, const StringValue& value) {
        return out << static_cast<const Value&>(value);
    }

    inline std::ostream& operator<<(std::ostream& out, const ArrayValue& value) {
        return out << static_cast<const Value&>(value);
    }
}
