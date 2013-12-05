#pragma once

#include <string>
#include <vector>
#include "common/boost_extension.hpp"

#define ALTO_NAMESPACE fastoredis

namespace fastoredis
{
    enum fastoType
    {
        ROOT = -1,
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
            enum ErrorsType { E_NONE, E_EXCEPTION, E_ERROR, E_INTERRUPTED };
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

    class FastoObject;

    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;

    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
    public:
        typedef unsigned int uint32_t;
        typedef char value_type;
        typedef std::vector<FastoObjectPtr> child_container_type;

        FastoObject(const FastoObjectPtr &parent, const fastoType &type, const value_type *memory);
        FastoObject(const FastoObjectPtr &parent, const fastoType &type, const value_type *memory, uint32_t size);

        ~FastoObject();

        fastoType type() const;
        uint32_t size() const;
        const value_type *const c_str() const;
        const value_type *begin()const;
        const value_type *end() const;
        void append(value_type c);

        static FastoObjectPtr createRoot();
        void addChildren(const FastoObjectPtr &child);

        friend std::string toStdString(const FastoObjectPtr &obj);
    private:
        void alloc(uint32_t strlen_result);
        FastoObject(); //only for root
        FastoObject(const FastoObject& other);
        FastoObject &operator=(const FastoObject &other);

        const FastoObjectPtr _parent;
        child_container_type _childrens;

        const fastoType _type;
        uint32_t alloc_size_;
        uint32_t size_;
        value_type *memory_;
    };

}
