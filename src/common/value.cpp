#include "common/value.h"

#include <algorithm>

#include "common/convert2string.h"

namespace common 
{
    namespace
    {
        Value* copyWithoutEmptyChildren(const Value* node)
        {
            DCHECK(node);
            switch (node->getType())
            {
                case Value::TYPE_ARRAY:
                {
                    const ArrayValue* list = static_cast<const ArrayValue*>(node);
                    ArrayValue* copy = new ArrayValue;
                    for (ArrayValue::const_iterator it = list->begin(); it != list->end(); ++it)
                    {
                        Value* child_copy = copyWithoutEmptyChildren(*it);
                        if (child_copy)
                            copy->append(child_copy);
                    }
                    if (!copy->empty())
                        return copy;

                    delete copy;
                        return NULL;
                }
                default:
                    // For everything else, just make a copy.
                    return node->deepCopy();
            }
        }

        const char *stringTypes[] = { "TYPE_NULL",
                                      "TYPE_BOOLEAN",
                                      "TYPE_INTEGER",
                                      "TYPE_UINTEGER",
                                      "TYPE_DOUBLE",
                                      "TYPE_STRING",
                                      "TYPE_ARRAY",
                                      "TYPE_STATUS",
                                      "TYPE_ERROR"
                                    };
    }

    class ValueEquals
    {
    public:
        explicit ValueEquals(const Value* first) : first_(first) { }

    bool operator ()(const Value* second) const {
        return first_->equals(second);
    }

    private:
        const Value* first_;
    };

    Value::~Value()
    {

    }

    string16 Value::toString() const
	{
        return string16();
	}

	// static
    Value* Value::createNullValue()
    {
        return new Value(TYPE_NULL);
	}

	// static
    FundamentalValue* Value::createBooleanValue(bool in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    FundamentalValue* Value::createIntegerValue(int in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    FundamentalValue* Value::createDoubleValue(double in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    StringValue* Value::createStringValue(const string16 &in_value)
    {
        return new StringValue(in_value);
	}

    StringValue* Value::createStringValue(const std::string& in_value)
    {
        return new StringValue(convertToString16(in_value));
    }

    // static
    ArrayValue* Value::createArrayValue()
    {
        return new ArrayValue();
    }

    ErrorValue* Value::createErrorValue(const string16 &in_value, Value::ErrorsType errorType, common::logging::LEVEL_LOG level)
    {
        return new ErrorValue(in_value, errorType, level);
    }

    ErrorValue* Value::createErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level)
    {
        return createErrorValue(convertToString16(in_value), errorType, level);
    }

    // static
    std::string Value::toString(Type t)
    {
        static const int count = SIZEOFMASS(stringTypes);
        if(t < count){
            return stringTypes[t];
        }
        return std::string();
    }

    bool Value::getAsBoolean(bool* out_value) const
    {
        return false;
	}

    bool Value::getAsInteger(int* out_value) const
    {
        return false;
	}

    bool Value::getAsUInteger(unsigned int* out_value) const
    {
        return false;
    }

    bool Value::getAsDouble(double* out_value) const
    {
        return false;
	}

    bool Value::getAsString(string16 *out_value) const
    {
        return false;
	}

    bool Value::getAsError(ErrorValue* out_value) const
    {
        return false;
    }

    bool Value::getAsList(ArrayValue** out_value)
    {
        return false;
	}

    bool Value::getAsList(const ArrayValue** out_value) const
    {
        return false;
	}

    Value* Value::deepCopy() const
    {
        DCHECK(isType(TYPE_NULL));
        return createNullValue();
	}

    bool Value::equals(const Value* other) const
    {
        DCHECK(isType(TYPE_NULL));
        return other->isType(TYPE_NULL);
	}

	// static
    bool Value::equals(const Value* a, const Value* b)
    {
        if ((a == NULL) && (b == NULL)) return true;
        if ((a == NULL) ^  (b == NULL)) return false;
        return a->equals(b);
	}

	Value::Value(Type type) : type_(type) {}

	Value::Value(const Value& that) : type_(that.type_) {}

    Value& Value::operator=(const Value& that)
    {
        type_ = that.type_;
        return *this;
	}

	///////////////////// FundamentalValue ////////////////////

	FundamentalValue::FundamentalValue(bool in_value)
        : Value(TYPE_BOOLEAN), boolean_value_(in_value)
    {
	}

	FundamentalValue::FundamentalValue(int in_value)
        : Value(TYPE_INTEGER), integer_value_(in_value)
    {
	}

    FundamentalValue::FundamentalValue(unsigned int in_value)
        : Value(TYPE_UINTEGER), uinteger_value_(in_value)
    {

    }

	FundamentalValue::FundamentalValue(double in_value)
        : Value(TYPE_DOUBLE), double_value_(in_value)
    {
	}

    FundamentalValue::~FundamentalValue()
    {
	}

    string16 FundamentalValue::toString() const
    {
        switch (getType()) {
            case TYPE_BOOLEAN:
            {
                return common::convertToString16(boolean_value_);
            }
            case TYPE_INTEGER:
            {
                return common::convertToString16(integer_value_);
            }
            case TYPE_UINTEGER:
            {
                return common::convertToString16(uinteger_value_);
            }
            case TYPE_DOUBLE:
            {
                return common::convertToString16(double_value_);
            }
            default:
                return string16();
        }
	}

    bool FundamentalValue::getAsBoolean(bool* out_value) const
    {
        if (out_value && isType(TYPE_BOOLEAN)){
            *out_value = boolean_value_;
        }
        return (isType(TYPE_BOOLEAN));
	}

    bool FundamentalValue::getAsInteger(int* out_value) const
    {
        if (out_value && isType(TYPE_INTEGER)){
            *out_value = integer_value_;
        }
        return (isType(TYPE_INTEGER));
	}

    bool FundamentalValue::getAsUInteger(unsigned int* out_value) const
    {
        if (out_value && isType(TYPE_UINTEGER)){
            *out_value = uinteger_value_;
        }
        return (isType(TYPE_UINTEGER));
    }

    bool FundamentalValue::getAsDouble(double* out_value) const
    {
        if (out_value && isType(TYPE_DOUBLE))
            *out_value = double_value_;
        else if (out_value && isType(TYPE_INTEGER))
            *out_value = integer_value_;
        else if (out_value && isType(TYPE_UINTEGER)){
            *out_value = uinteger_value_;
        }
        return (isType(TYPE_DOUBLE) || isType(TYPE_INTEGER) || isType(TYPE_UINTEGER));
    }

    FundamentalValue* FundamentalValue::deepCopy() const {
      switch (getType()) {
		case TYPE_BOOLEAN:
          return createBooleanValue(boolean_value_);

		case TYPE_INTEGER:
          return createIntegerValue(integer_value_);

		case TYPE_DOUBLE:
          return createDoubleValue(double_value_);

        default:
		  return NULL;
	  }
	}

    bool FundamentalValue::equals(const Value* other) const
    {
        if (other->getType() != getType())
            return false;

        switch (getType()) {
            case TYPE_BOOLEAN: {
            bool lhs, rhs;
                return getAsBoolean(&lhs) && other->getAsBoolean(&rhs) && lhs == rhs;
            }
            case TYPE_INTEGER: {
            int lhs, rhs;
                return getAsInteger(&lhs) && other->getAsInteger(&rhs) && lhs == rhs;
            }
            case TYPE_DOUBLE: {
            double lhs, rhs;
                return getAsDouble(&lhs) && other->getAsDouble(&rhs) && lhs == rhs;
            }
            default:
                return false;
        }
	}

	///////////////////// StringValue ////////////////////

    StringValue::StringValue(const std::string& in_value)
        : Value(TYPE_STRING),
          value_(convertToString16(in_value))
    {

    }

    StringValue::StringValue(const string16 &in_value)
		: Value(TYPE_STRING),
          value_(in_value)
    {
	}

    StringValue::~StringValue()
    {
	}

    string16 StringValue::toString() const
	{
		return value_;
	}

    bool StringValue::getAsString(string16 *out_value) const
    {
	  if (out_value)
		*out_value = value_;
	  return true;
    }

    StringValue* StringValue::deepCopy() const
    {
        return createStringValue(value_);
	}

    bool StringValue::equals(const Value* other) const
    {
        if (other->getType() != getType())
            return false;
        string16 lhs, rhs;
        return getAsString(&lhs) && other->getAsString(&rhs) && lhs == rhs;
    }

    ArrayValue::ArrayValue() : Value(TYPE_ARRAY)
    {
	}

    ArrayValue::~ArrayValue()
    {
        clear();
	}

    void ArrayValue::clear()
    {
        for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i)
            delete *i;
        list_.clear();
	}

    bool ArrayValue::set(size_t index, Value* in_value)
    {
        if (!in_value)
            return false;

        if (index >= list_.size()) {
        // Pad out any intermediate indexes with null settings
        while (index > list_.size())
            append(createNullValue());
            append(in_value);
            } else {
            DCHECK(list_[index] != in_value);
            delete list_[index];
            list_[index] = in_value;
        }
        return true;
	}

    bool ArrayValue::get(size_t index, const Value** out_value) const
    {
        if (index >= list_.size())
            return false;

        if (out_value)
        *out_value = list_[index];

        return true;
	}

    bool ArrayValue::get(size_t index, Value** out_value)
    {
        return static_cast<const ArrayValue&>(*this).get(
            index,
            const_cast<const Value**>(out_value));
	}

    bool ArrayValue::getBoolean(size_t index, bool* bool_value) const
    {
        const Value* value;
        if (!get(index, &value))
            return false;

        return value->getAsBoolean(bool_value);
	}

    bool ArrayValue::getInteger(size_t index, int* out_value) const
    {
        const Value* value;
        if (!get(index, &value))
            return false;

        return value->getAsInteger(out_value);
	}

    bool ArrayValue::getDouble(size_t index, double* out_value) const
    {
        const Value* value;
        if (!get(index, &value))
            return false;

        return value->getAsDouble(out_value);
	}

    bool ArrayValue::getString(size_t index, string16 *out_value) const
    {
        const Value* value;
        if (!get(index, &value))
            return false;

        return value->getAsString(out_value);
    }

    bool ArrayValue::getList(size_t index, const ArrayValue** out_value) const
    {
        const Value* value;
        bool result = get(index, &value);
        if (!result || !value->isType(TYPE_ARRAY))
            return false;

        if (out_value)
            *out_value = static_cast<const ArrayValue*>(value);

        return true;
	}

    bool ArrayValue::getList(size_t index, ArrayValue** out_value)
    {
        return static_cast<const ArrayValue&>(*this).getList(
            index,
            const_cast<const ArrayValue**>(out_value));
	}

    bool ArrayValue::remove(size_t index, boost::scoped_ptr<Value>* out_value)
    {
        if (index >= list_.size())
            return false;

        if (out_value)
            out_value->reset(list_[index]);
        else
            delete list_[index];

        list_.erase(list_.begin() + index);
        return true;
	}

    bool ArrayValue::remove(const Value& value, size_t* index)
    {
        for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i) {
            if ((*i)->equals(&value)) {
                size_t previous_index = i - list_.begin();
                delete *i;
                list_.erase(i);

                if (index)
                *index = previous_index;
                return true;
            }
        }
        return false;
	}

    ArrayValue::iterator ArrayValue::erase(iterator iter,
                                         boost::scoped_ptr<Value>* out_value)
    {
        if (out_value)
            out_value->reset(*iter);
        else
            delete *iter;

        return list_.erase(iter);
	}

    void ArrayValue::append(Value* in_value)
    {
        DCHECK(in_value);
        list_.push_back(in_value);
	}

    void ArrayValue::appendBoolean(bool in_value)
    {
        append(createBooleanValue(in_value));
	}

    void ArrayValue::appendInteger(int in_value)
    {
        append(createIntegerValue(in_value));
	}

    void ArrayValue::appendDouble(double in_value)
    {
        append(createDoubleValue(in_value));
	}

    void ArrayValue::appendString(const string16 &in_value)
    {
        append(createStringValue(in_value));
    }

    void ArrayValue::appendStrings(const std::vector<string16>& in_values)
    {
        for (std::vector<string16>::const_iterator it = in_values.begin();
        it != in_values.end(); ++it) {
            appendString(*it);
        }
    }

    bool ArrayValue::appendIfNotPresent(Value* in_value)
    {
        DCHECK(in_value);
        for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i) {
            if ((*i)->equals(in_value)) {
                delete in_value;
                return false;
            }
        }
        list_.push_back(in_value);
        return true;
	}

    bool ArrayValue::insert(size_t index, Value* in_value)
    {
        DCHECK(in_value);
        if (index > list_.size())
            return false;

        list_.insert(list_.begin() + index, in_value);
        return true;
	}

    ArrayValue::const_iterator ArrayValue::find(const Value& value) const
    {
        return std::find_if(list_.begin(), list_.end(), ValueEquals(&value));
	}

    void ArrayValue::swap(ArrayValue* other)
    {
        list_.swap(other->list_);
	}

    bool ArrayValue::getAsList(ArrayValue** out_value)
    {
        if (out_value)
            *out_value = this;
        return true;
	}

    bool ArrayValue::getAsList(const ArrayValue** out_value) const
    {
        if (out_value)
            *out_value = this;
        return true;
	}

    ArrayValue* ArrayValue::deepCopy() const
    {
        ArrayValue* result = new ArrayValue;

        for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i){
            result->append((*i)->deepCopy());
        }

        return result;
	}

    bool ArrayValue::equals(const Value* other) const
    {
        if (other->getType() != getType())
            return false;

        const ArrayValue* other_list =
        static_cast<const ArrayValue*>(other);
        const_iterator lhs_it, rhs_it;
        for (lhs_it = begin(), rhs_it = other_list->begin();
        lhs_it != end() && rhs_it != other_list->end();
        ++lhs_it, ++rhs_it) {
        if (!(*lhs_it)->equals(*rhs_it))
            return false;
        }
        if (lhs_it != end() || rhs_it != other_list->end())
            return false;

        return true;
	}

    string16 ArrayValue::toString() const
	{
        string16 result;

		for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i)
			result += (*i)->toString();

		return result;
    }

    ErrorValue::ErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level)
        : Value(TYPE_ERROR), description_(convertToString16(in_value)), errorType_(errorType), level_(level)
    {

    }

    ErrorValue::ErrorValue(const string16 &in_value, ErrorsType errorType, common::logging::LEVEL_LOG level)
        : Value(TYPE_ERROR), description_(in_value), errorType_(errorType), level_(level)
    {
    }

    ErrorValue::ErrorValue()
        : Value(TYPE_ERROR), description_(), errorType_(E_NONE), level_(common::logging::L_NONE)
    {

    }

    ErrorValue::~ErrorValue()
    {

    }

    bool ErrorValue::isError() const
    {
        return errorType_ != E_NONE;
    }

    common::logging::LEVEL_LOG ErrorValue::level() const
    {
        return level_;
    }

    string16 ErrorValue::description() const
    {
        return description_;
    }

    string16 ErrorValue::toString() const
	{
		return description();
	}

    bool ErrorValue::getAsError(ErrorValue* out_value) const
    {
        if (out_value){
            (*out_value).description_ = description_;
            (*out_value).errorType_ = errorType_;
            (*out_value).level_ = level_;
        }
        return true;
    }
}
