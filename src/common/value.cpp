#include "common/value.h"

#include <algorithm>
#include <memory>

namespace common 
{

    namespace
    {
        Value* CopyWithoutEmptyChildren(const Value* node)
        {
            DCHECK(node);
            switch (node->GetType())
            {
                case Value::TYPE_ARRAY:
                {
                    const ArrayValue* list = static_cast<const ArrayValue*>(node);
                    ArrayValue* copy = new ArrayValue;
                    for (ArrayValue::const_iterator it = list->begin(); it != list->end();
                    ++it) {
                        Value* child_copy = CopyWithoutEmptyChildren(*it);
                    if (child_copy)
                        copy->Append(child_copy);
                    }
                    if (!copy->empty())
                        return copy;

                    delete copy;
                        return NULL;
                }
                default:
                    // For everything else, just make a copy.
                    return node->DeepCopy();
            }
        }
    }

    class ValueEquals
    {
    public:
        explicit ValueEquals(const Value* first) : first_(first) { }

    bool operator ()(const Value* second) const {
        return first_->Equals(second);
    }

    private:
        const Value* first_;
    };

    Value::~Value()
    {

    }

	// static
    Value* Value::CreateNullValue()
    {
        return new Value(TYPE_NULL);
	}

	// static
    FundamentalValue* Value::CreateBooleanValue(bool in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    FundamentalValue* Value::CreateIntegerValue(int in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    FundamentalValue* Value::CreateDoubleValue(double in_value)
    {
        return new FundamentalValue(in_value);
	}

	// static
    StringValue* Value::CreateStringValue(const std::string& in_value)
    {
        return new StringValue(in_value);
	}

    // static
    ArrayValue* Value::CreateArrayValue()
    {
        return new ArrayValue();
    }

    ErrorValue* Value::CreateErrorValue(const std::string& in_value, Value::ErrorsType errorType, common::logging::LEVEL_LOG level)
    {
        return new ErrorValue(in_value, errorType, level);
    }

    // static
    std::string Value::toStdString(Type t)
    {
#pragma message("plz implement")
        return "";
    }

    bool Value::GetAsBoolean(bool* out_value) const
    {
        return false;
	}

    bool Value::GetAsInteger(int* out_value) const
    {
        return false;
	}

    bool Value::GetAsDouble(double* out_value) const
    {
        return false;
	}

    bool Value::GetAsString(std::string* out_value) const
    {
        return false;
	}

    bool Value::GetAsError(ErrorValue* out_value) const
    {
        return false;
    }

    bool Value::GetAsList(ArrayValue** out_value)
    {
        return false;
	}

    bool Value::GetAsList(const ArrayValue** out_value) const
    {
        return false;
	}

    Value* Value::DeepCopy() const
    {
        DCHECK(IsType(TYPE_NULL));
        return CreateNullValue();
	}

    bool Value::Equals(const Value* other) const
    {
        DCHECK(IsType(TYPE_NULL));
        return other->IsType(TYPE_NULL);
	}

	// static
    bool Value::Equals(const Value* a, const Value* b)
    {
        if ((a == NULL) && (b == NULL)) return true;
        if ((a == NULL) ^  (b == NULL)) return false;
        return a->Equals(b);
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

	FundamentalValue::FundamentalValue(double in_value)
        : Value(TYPE_DOUBLE), double_value_(in_value)
    {
	}

    FundamentalValue::~FundamentalValue()
    {
	}

    bool FundamentalValue::GetAsBoolean(bool* out_value) const
    {
        if (out_value && IsType(TYPE_BOOLEAN))
            *out_value = boolean_value_;
        return (IsType(TYPE_BOOLEAN));
	}

    bool FundamentalValue::GetAsInteger(int* out_value) const
    {
        if (out_value && IsType(TYPE_INTEGER))
            *out_value = integer_value_;
        return (IsType(TYPE_INTEGER));
	}

    bool FundamentalValue::GetAsDouble(double* out_value) const
    {
        if (out_value && IsType(TYPE_DOUBLE))
            *out_value = double_value_;
        else if (out_value && IsType(TYPE_INTEGER))
            *out_value = integer_value_;
        return (IsType(TYPE_DOUBLE) || IsType(TYPE_INTEGER));
	}

	FundamentalValue* FundamentalValue::DeepCopy() const {
	  switch (GetType()) {
		case TYPE_BOOLEAN:
		  return CreateBooleanValue(boolean_value_);

		case TYPE_INTEGER:
		  return CreateIntegerValue(integer_value_);

		case TYPE_DOUBLE:
		  return CreateDoubleValue(double_value_);

        default:
		  return NULL;
	  }
	}

    bool FundamentalValue::Equals(const Value* other) const
    {
        if (other->GetType() != GetType())
            return false;

        switch (GetType()) {
            case TYPE_BOOLEAN: {
            bool lhs, rhs;
                return GetAsBoolean(&lhs) && other->GetAsBoolean(&rhs) && lhs == rhs;
            }
            case TYPE_INTEGER: {
            int lhs, rhs;
                return GetAsInteger(&lhs) && other->GetAsInteger(&rhs) && lhs == rhs;
            }
            case TYPE_DOUBLE: {
            double lhs, rhs;
                return GetAsDouble(&lhs) && other->GetAsDouble(&rhs) && lhs == rhs;
            }
            default:
                return false;
        }
	}

	///////////////////// StringValue ////////////////////

    StringValue::StringValue(const std::string& in_value)
		: Value(TYPE_STRING),
          value_(in_value)
    {
	}

    StringValue::~StringValue()
    {
	}

    bool StringValue::GetAsString(std::string* out_value) const
    {
	  if (out_value)
		*out_value = value_;
	  return true;
    }

    StringValue* StringValue::DeepCopy() const
    {
        return CreateStringValue(value_);
	}

    bool StringValue::Equals(const Value* other) const
    {
        if (other->GetType() != GetType())
            return false;
        std::string lhs, rhs;
        return GetAsString(&lhs) && other->GetAsString(&rhs) && lhs == rhs;
    }

    ArrayValue::ArrayValue() : Value(TYPE_ARRAY)
    {
	}

    ArrayValue::~ArrayValue()
    {
        Clear();
	}

    void ArrayValue::Clear()
    {
        for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i)
            delete *i;
        list_.clear();
	}

    bool ArrayValue::Set(size_t index, Value* in_value)
    {
        if (!in_value)
            return false;

        if (index >= list_.size()) {
        // Pad out any intermediate indexes with null settings
        while (index > list_.size())
            Append(CreateNullValue());
            Append(in_value);
            } else {
            DCHECK(list_[index] != in_value);
            delete list_[index];
            list_[index] = in_value;
        }
        return true;
	}

    bool ArrayValue::Get(size_t index, const Value** out_value) const
    {
        if (index >= list_.size())
            return false;

        if (out_value)
        *out_value = list_[index];

        return true;
	}

    bool ArrayValue::Get(size_t index, Value** out_value)
    {
        return static_cast<const ArrayValue&>(*this).Get(
            index,
            const_cast<const Value**>(out_value));
	}

    bool ArrayValue::GetBoolean(size_t index, bool* bool_value) const
    {
        const Value* value;
        if (!Get(index, &value))
            return false;

        return value->GetAsBoolean(bool_value);
	}

    bool ArrayValue::GetInteger(size_t index, int* out_value) const
    {
        const Value* value;
        if (!Get(index, &value))
            return false;

        return value->GetAsInteger(out_value);
	}

    bool ArrayValue::GetDouble(size_t index, double* out_value) const
    {
        const Value* value;
        if (!Get(index, &value))
            return false;

        return value->GetAsDouble(out_value);
	}

    bool ArrayValue::GetString(size_t index, std::string* out_value) const
    {
        const Value* value;
        if (!Get(index, &value))
            return false;

        return value->GetAsString(out_value);
    }

    bool ArrayValue::GetList(size_t index, const ArrayValue** out_value) const
    {
        const Value* value;
        bool result = Get(index, &value);
        if (!result || !value->IsType(TYPE_ARRAY))
            return false;

        if (out_value)
            *out_value = static_cast<const ArrayValue*>(value);

        return true;
	}

    bool ArrayValue::GetList(size_t index, ArrayValue** out_value)
    {
        return static_cast<const ArrayValue&>(*this).GetList(
            index,
            const_cast<const ArrayValue**>(out_value));
	}

    bool ArrayValue::Remove(size_t index, boost::scoped_ptr<Value>* out_value)
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

    bool ArrayValue::Remove(const Value& value, size_t* index)
    {
        for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i) {
            if ((*i)->Equals(&value)) {
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

    ArrayValue::iterator ArrayValue::Erase(iterator iter,
                                         boost::scoped_ptr<Value>* out_value)
    {
        if (out_value)
            out_value->reset(*iter);
        else
            delete *iter;

        return list_.erase(iter);
	}

    void ArrayValue::Append(Value* in_value)
    {
        DCHECK(in_value);
        list_.push_back(in_value);
	}

    void ArrayValue::AppendBoolean(bool in_value)
    {
        Append(CreateBooleanValue(in_value));
	}

    void ArrayValue::AppendInteger(int in_value)
    {
        Append(CreateIntegerValue(in_value));
	}

    void ArrayValue::AppendDouble(double in_value)
    {
        Append(CreateDoubleValue(in_value));
	}

    void ArrayValue::AppendString(const std::string& in_value)
    {
        Append(CreateStringValue(in_value));
    }

    void ArrayValue::AppendStrings(const std::vector<std::string>& in_values)
    {
        for (std::vector<std::string>::const_iterator it = in_values.begin();
        it != in_values.end(); ++it) {
            AppendString(*it);
        }
    }

    bool ArrayValue::AppendIfNotPresent(Value* in_value)
    {
        DCHECK(in_value);
        for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i) {
            if ((*i)->Equals(in_value)) {
                delete in_value;
                return false;
            }
        }
        list_.push_back(in_value);
        return true;
	}

    bool ArrayValue::Insert(size_t index, Value* in_value)
    {
        DCHECK(in_value);
        if (index > list_.size())
            return false;

        list_.insert(list_.begin() + index, in_value);
        return true;
	}

    ArrayValue::const_iterator ArrayValue::Find(const Value& value) const
    {
        return std::find_if(list_.begin(), list_.end(), ValueEquals(&value));
	}

    void ArrayValue::Swap(ArrayValue* other)
    {
        list_.swap(other->list_);
	}

    bool ArrayValue::GetAsList(ArrayValue** out_value)
    {
        if (out_value)
            *out_value = this;
        return true;
	}

    bool ArrayValue::GetAsList(const ArrayValue** out_value) const
    {
        if (out_value)
            *out_value = this;
        return true;
	}

    ArrayValue* ArrayValue::DeepCopy() const
    {
        ArrayValue* result = new ArrayValue;

        for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i)
            result->Append((*i)->DeepCopy());

        return result;
	}

    bool ArrayValue::Equals(const Value* other) const
    {
        if (other->GetType() != GetType())
            return false;

        const ArrayValue* other_list =
        static_cast<const ArrayValue*>(other);
        const_iterator lhs_it, rhs_it;
        for (lhs_it = begin(), rhs_it = other_list->begin();
        lhs_it != end() && rhs_it != other_list->end();
        ++lhs_it, ++rhs_it) {
        if (!(*lhs_it)->Equals(*rhs_it))
            return false;
        }
        if (lhs_it != end() || rhs_it != other_list->end())
            return false;

        return true;
	}

    ErrorValue::ErrorValue(const std::string& in_value, ErrorsType errorType, common::logging::LEVEL_LOG level)
        : Value(TYPE_ERROR), description_(in_value), errorType_(errorType), level_(level)
    {
    }

    ErrorValue::ErrorValue()
        : Value(TYPE_ERROR), description_(), errorType_(E_NONE), level_(common::logging::NONE)
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

    std::string ErrorValue::description() const
    {
        return description_;
    }

    bool ErrorValue::GetAsError(ErrorValue* out_value) const
    {
        if (out_value){
            (*out_value).description_ = description_;
            (*out_value).errorType_ = errorType_;
            (*out_value).level_ = level_;
        }
        return true;
    }
}
