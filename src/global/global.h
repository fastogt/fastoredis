#pragma once

#include <string>
#include <vector>

#include "common/log_levels.hpp"
#include "common/boost_extension.hpp"

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
        ERROR = 6,
        ROOT = 7
    };

    enum supportedViews
    {
        Tree = 0,
        Table,
        Text
    };

    namespace error
    {
        struct ErrorInfo
        {
            enum ErrorsType { E_NONE, E_EXCEPTION, E_ERROR, E_INTERRUPTED };
            ErrorInfo():_description(), _errorType(E_NONE), _level(common::logging::NONE){}
            ErrorInfo(const std::string &desc, ErrorsType errorType, common::logging::LEVEL_LOG level = common::logging::WARNING)
                :_description(desc), _errorType(errorType), _level(level) {}

            bool isError() const { return _errorType != E_NONE; }

            std::string _description;
            ErrorsType _errorType;
            common::logging::LEVEL_LOG _level;
        };
    }

    std::string toStdString(fastoType t);
    fastoType toFastoType(const std::string &text);

    std::string toStdString(supportedViews v);
    supportedViews toSupportedViews(const std::string &text);
    std::vector<std::string> allSupportedViews();

    class FastoObject;

    typedef boost::intrusive_ptr<FastoObject> FastoObjectPtr;

    std::string toStdString(const FastoObjectPtr &obj);

    class FastoObject
            : public common::boost_extension::intrusive_ptr_base<FastoObject>
    {
    public:
        typedef unsigned int uint32_t;
        typedef char value_type;
        typedef std::vector<FastoObjectPtr> child_container_type;

        FastoObject(const FastoObjectPtr &parent, const value_type *memory, const fastoType &type);
        FastoObject(const FastoObjectPtr &parent, const value_type *memory, uint32_t size, const fastoType &type);

        ~FastoObject();

        fastoType type() const;
        uint32_t size() const;
        const value_type *const c_str() const;
        const value_type *begin()const;
        const value_type *end() const;
        void append(value_type c);

        child_container_type childrens() const;
        static FastoObjectPtr createRoot(const value_type *memory);
        void addChildren(const FastoObjectPtr &child);
        bool isRoot() const;

    private:
        void alloc(uint32_t strlen_result);
        FastoObject(const FastoObject& other);
        FastoObject &operator=(const FastoObject &other);

        const FastoObjectPtr _parent;
        child_container_type _childrens;

        uint32_t alloc_size_;
        uint32_t size_;
        value_type *memory_;
        const fastoType _type;
    };
}
