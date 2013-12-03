#pragma once

#include <string>
#include <vector>
#include "common/boost_extension.hpp"

#define ALTO_NAMESPACE fastoredis

namespace fastoredis
{
    enum fastoType
    {
        UNKNOWN = 0,
        STRING = 1,
        ARRAY = 2,
        INTEGER = 3,
        NIL = 4,
        STATUS = 5,
        ERROR = 6
    };

    namespace error
    {
        struct ErrorInfo
        {
            enum ErrorsType { E_NONE, E_EXCEPTION, E_ERROR, E_INTERUPTED };
            ErrorInfo():_description(),_errorType(E_NONE){}
            ErrorInfo(const std::string &desc, ErrorsType errorType):_description(desc),_errorType(errorType) {}
            bool isError() const { return _errorType != E_NONE; }
            std::string _description;
            ErrorsType _errorType;
        };
    }

    namespace detail
    {
        template<fastoType type>
        struct FastoTraits;

        template<>
        struct FastoTraits<UNKNOWN>
        {
            typedef void* type_t;
        };
        template<>
        struct FastoTraits<STRING>
        {
            typedef std::string type_t;
        };
        template<>
        struct FastoTraits<ARRAY>
        {
            typedef std::vector<std::string> type_t;
        };
        template<>
        struct FastoTraits<INTEGER>
        {
            typedef int type_t;
        };
        template<>
        struct FastoTraits<NIL>
        {
            typedef void* type_t;
        };
        template<>
        struct FastoTraits<STATUS>
        {
            typedef void* type_t;
        };
        template<>
        struct FastoTraits<ERROR>
        {
            typedef error::ErrorInfo type_t;
        };
    }

    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
    public:
        typedef unsigned int uint32_t;
        typedef char value_type;

        explicit FastoObject(const fastoType &type, const value_type *memory);

        ~FastoObject();

        fastoType type() const;
        uint32_t size() const;
        const value_type *const c_str() const;
        const value_type *begin()const;
        const value_type *end() const;

    private:
        FastoObject(const FastoObject& other);
        FastoObject& operator=(const FastoObject &other);

        const fastoType _type;
        uint32_t size_;
        value_type *memory_;
    };

    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;
}
