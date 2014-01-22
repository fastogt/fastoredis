#pragma once

#include "common/log_levels.hpp"
#include <boost/scoped_ptr.hpp>
#include <vector>

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
            TYPE_DOUBLE,
            TYPE_STRING,
            TYPE_ARRAY,
            TYPE_STATUS,
			TYPE_ERROR
        };

        enum ErrorsType { E_NONE, E_EXCEPTION, E_ERROR, E_INTERRUPTED };

        virtual ~Value();

        static Value* CreateNullValue();

        static FundamentalValue* CreateBooleanValue(bool in_value);
        static FundamentalValue* CreateIntegerValue(int in_value);
        static FundamentalValue* CreateDoubleValue(double in_value);
        static StringValue* CreateStringValue(const std::string& in_value);
        static ArrayValue* CreateArrayValue();
        static ErrorValue* CreateErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level);

        static std::string toStdString(Type t);
		virtual std::string toStdString() const;
        Type GetType() const { return type_; }

        bool IsType(Type type) const { return type == type_; }

        virtual bool GetAsBoolean(bool* out_value) const;
        virtual bool GetAsInteger(int* out_value) const;
        virtual bool GetAsDouble(double* out_value) const;
        virtual bool GetAsString(std::string* out_value) const;
        virtual bool GetAsError(ErrorValue* out_value) const;
        virtual bool GetAsList(ArrayValue** out_value);
        virtual bool GetAsList(const ArrayValue** out_value) const;

        virtual Value* DeepCopy() const;

        virtual bool Equals(const Value* other) const;

        static bool Equals(const Value* a, const Value* b);

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
        explicit FundamentalValue(double in_value);
        virtual ~FundamentalValue();

		virtual std::string toStdString() const;
        virtual bool GetAsBoolean(bool* out_value) const;
        virtual bool GetAsInteger(int* out_value) const;
        virtual bool GetAsDouble(double* out_value) const;
        virtual FundamentalValue* DeepCopy() const;
        virtual bool Equals(const Value* other) const;

    private:
		DISALLOW_COPY_AND_ASSIGN(FundamentalValue);
        union {
        bool boolean_value_;
        int integer_value_;
        double double_value_;
        };
    };

    class StringValue : public Value
    {
        public:
            explicit StringValue(const std::string& in_value);
            virtual ~StringValue();

			virtual std::string toStdString() const;
            virtual bool GetAsString(std::string* out_value) const;
            virtual StringValue* DeepCopy() const;
            virtual bool Equals(const Value* other) const;

        private:
			DISALLOW_COPY_AND_ASSIGN(StringValue);
            std::string value_;
    };

    class ArrayValue : public Value
    {
    public:
        typedef ValueVector::iterator iterator;
        typedef ValueVector::const_iterator const_iterator;

        ArrayValue();
        virtual ~ArrayValue();

		virtual std::string toStdString() const;
        void Clear();

        size_t GetSize() const { return list_.size(); }

        // Returns whether the list is empty.
        bool empty() const { return list_.empty(); }

        bool Set(size_t index, Value* in_value);

        bool Get(size_t index, const Value** out_value) const;
        bool Get(size_t index, Value** out_value);

        bool GetBoolean(size_t index, bool* out_value) const;
        bool GetInteger(size_t index, int* out_value) const;
        bool GetDouble(size_t index, double* out_value) const;
        bool GetString(size_t index, std::string* out_value) const;
        bool GetList(size_t index, const ArrayValue** out_value) const;
        bool GetList(size_t index, ArrayValue** out_value);

        virtual bool Remove(size_t index, boost::scoped_ptr<Value>* out_value);

        bool Remove(const Value& value, size_t* index);

        iterator Erase(iterator iter, boost::scoped_ptr<Value>* out_value);

        // Appends a Value to the end of the list.
        void Append(Value* in_value);

        // Convenience forms of Append.
        void AppendBoolean(bool in_value);
        void AppendInteger(int in_value);
        void AppendDouble(double in_value);
        void AppendString(const std::string& in_value);
        void AppendStrings(const std::vector<std::string>& in_values);

        bool AppendIfNotPresent(Value* in_value);

        bool Insert(size_t index, Value* in_value);

        const_iterator Find(const Value& value) const;

        // Swaps contents with the |other| list.
        virtual void Swap(ArrayValue* other);

        // Iteration.
        iterator begin() { return list_.begin(); }
        iterator end() { return list_.end(); }

        const_iterator begin() const { return list_.begin(); }
        const_iterator end() const { return list_.end(); }

        virtual bool GetAsList(ArrayValue** out_value);
        virtual bool GetAsList(const ArrayValue** out_value) const;
        virtual ArrayValue* DeepCopy() const;
        virtual bool Equals(const Value* other) const;

    private:
		DISALLOW_COPY_AND_ASSIGN(ArrayValue);
        ValueVector list_;
    };

    class ErrorValue : public Value
    {
    public:
        explicit ErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level = common::logging::WARNING);
        ErrorValue();
        bool isError() const;
        common::logging::LEVEL_LOG level() const;
        std::string description() const;
		virtual std::string toStdString() const;

		virtual bool GetAsError(ErrorValue* out_value) const;
        virtual ~ErrorValue();

    private:
		//DISALLOW_COPY_AND_ASSIGN(ErrorValue);
        std::string description_;
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
